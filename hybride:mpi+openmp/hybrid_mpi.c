/**
 * APPROXIMATE PATTERN MATCHING
 *
 * INF560 X2016
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>

#define APM_DEBUG 0
char *
read_input_file( char * filename, int * size )
{
    char * buf ;
    off_t fsize;
    int fd = 0 ;
    int n_bytes = 1 ;

    /* Open the text file */
    fd = open( filename, O_RDONLY ) ;
    if ( fd == -1 )
    {
        fprintf( stderr, "Unable to open the text file <%s>\n", filename ) ;
        return NULL ;
    }


    /* Get the number of characters in the textfile */
    fsize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);


#if APM_DEBUG
    printf( "File length: %lld\n", fsize ) ;
#endif

    /* Allocate data to copy the target text */
    buf = (char *)malloc( fsize * sizeof ( char ) ) ;
    if ( buf == NULL )
    {
        fprintf( stderr, "Unable to allocate %lld byte(s) for main array\n",
                fsize ) ;
        return NULL ;
    }

    n_bytes = read( fd, buf, fsize ) ;
    if ( n_bytes != fsize )
    {
        fprintf( stderr,
                "Unable to copy %lld byte(s) from text file (%d byte(s) copied)\n",
                fsize, n_bytes) ;
        return NULL ;
    }

#if APM_DEBUG
    printf( "Number of read bytes: %d\n", n_bytes ) ;
#endif

    *size = n_bytes ;


    close( fd ) ;


    return buf ;
}


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2, int len, int * column) {
    unsigned int x, y, lastdiag, olddiag;

    for (y = 1; y <= len; y++)
    {
        column[y] = y;
    }
    for (x = 1; x <= len; x++) {
        column[0] = x;
        lastdiag = x-1 ;
        for (y = 1; y <= len; y++) {
            olddiag = column[y];
            column[y] = MIN3(
                    column[y] + 1,
                    column[y-1] + 1,
                    lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1)
                    );
            lastdiag = olddiag;

        }
    }
    return(column[len]);
}




int
main( int argc, char ** argv )
{
  char * filename ;
  int n_bytes ;
  char * buf ;
  int * n_matches ;
  int comm_size,me;
  int * MPI_THREAD_PROVIDED ;
  MPI_Init_thread(&argc, &argv,MPI_THREAD_FUNNELED, MPI_THREAD_PROVIDED);
  MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&me);

  char ** pattern ;
  int approx_factor = 0 ;
  int nb_patterns = 0 ;
  int i, j ;
  struct timeval t1, t2;
  double duration ;

  /* Check number of arguments */
  if ( argc < 4 )
  {
    printf( "Usage: %s approximation_factor "
            "dna_database pattern1 pattern2 ...\n",
            argv[0] ) ;
    return 1 ;
  }

  /* Get the distance factor */
  approx_factor = atoi( argv[1] ) ;

  /* Grab the filename containing the target text */
  filename = argv[2] ;

  /* Get the number of patterns that the user wants to search for */
  nb_patterns = argc - 3 ;

  /* Fill the pattern array */
  pattern = (char **)malloc( nb_patterns * sizeof( char * ) ) ;
  if ( pattern == NULL )
  {
      fprintf( stderr,
              "Unable to allocate array of pattern of size %d\n",
              nb_patterns ) ;
      return 1 ;
  }

  /* Grab the patterns */
  for ( i = 0 ; i < nb_patterns ; i++ )
  {
      int l ;
      l = strlen(argv[i+3]) ;
      if ( l <= 0 )
      {
          fprintf( stderr, "Error while parsing argument %d\n", i+3 ) ;
          return 1 ;
      }

      pattern[i] = (char *)malloc( (l+1) * sizeof( char ) ) ;
      if ( pattern[i] == NULL )
      {
          fprintf( stderr, "Unable to allocate string of size %d\n", l ) ;
          return 1 ;
      }

      strncpy( pattern[i], argv[i+3], (l+1) ) ;

  }
if(me==0){
printf( "Approximate Pattern Mathing: "
          "looking for %d pattern(s) in file %s w/ distance of %d\n",
          nb_patterns, filename, approx_factor ) ;
  buf = read_input_file( filename, &n_bytes ) ;
  if ( buf == NULL )
  {
      return 1 ;
  }
  /* Allocate the array of matches */
  n_matches = (int *)malloc( nb_patterns * sizeof( long long int ) ) ;
  if ( n_matches == NULL )
  {
      fprintf( stderr, "Error: unable to allocate memory for %ldB\n",
              nb_patterns * sizeof( int ) ) ;
      return 1 ;
  }

  for ( int i = 0 ; i < nb_patterns; i++ )
  {
    n_matches[i] = 0 ;
    int s = 0 ;
    int p_size = strlen(pattern[i]) ;
    int * columnj = (int *)malloc( (p_size+1) * sizeof( int ) ) ;

    if ( columnj == NULL )
    {
        fprintf( stderr, "Error: unable to allocate memory for column (%ldB)\n",
                (p_size+1) * sizeof( int ) ) ;
        return 1 ;
    }
  // iterate over processes by rank
  for (int r=1;r<comm_size;r++)
  {
    char borders[2*p_size - 2 ];
    int k=0;
    for (int j=p_size - 1 ;j>0;j-- )
    {
      borders[k]=buf[(n_bytes/comm_size)*r - j];
      k++;
    }
    for (int j=0;j<p_size - 1  ;j++ )
    {
      borders[k]=buf[(n_bytes/comm_size)*r + j];
      k++;
    }

    for ( int j = 0 ; j < 2*p_size -2 ; j++ )
    {
      int  size = p_size ;
      if ( 2*p_size - 2  - j < p_size  )
      {
          size = 2*p_size - 2 - j  ;
      }
       int distance = levenshtein( pattern[i], &borders[j], size, columnj ) + p_size - size ;

      if ( distance <= approx_factor ) {
           s++ ;
      }
  }
}
n_matches [i] = s;
free(columnj);
	}
}



  /*****
   * BEGIN MAIN LOOP
   ******/

  //broacast size of file to all processes
  MPI_Bcast(&n_bytes,1,MPI_INT,0,MPI_COMM_WORLD);
  int sendcount = n_bytes/comm_size;
  char* bufloc = malloc(sizeof(char)*sendcount)  ;
  int matched,total;
  //sends chunks of the file to different processes.
  MPI_Scatter(buf, sendcount, MPI_CHAR,bufloc , sendcount, MPI_CHAR , 0 ,MPI_COMM_WORLD);
 /* Timer start */
  if (me == 0 ) {
   gettimeofday(&t1, NULL);
  }

  for ( i = 0 ; i < nb_patterns ; i++ )
  {
      matched = 0;
      int size_pattern = strlen(pattern[i]) ;

      int * column ;

      column = (int *)malloc( (size_pattern+1) * sizeof( int ) ) ;
      if ( column == NULL )
      {
          fprintf( stderr, "Error: unable to allocate memory for column (%ldB)\n",
                  (size_pattern+1) * sizeof( int ) ) ;
          return 1 ;
      }
      int found =0;
        #pragma omp parallel for schedule(dynamic) reduction(+:found)
      for ( j = 0 ; j < sendcount  ; j++ )
      {

          int distance = 0 ;
          int size ;

#if APM_DEBUG
          if ( j % 100 == 0 )
          {
          printf( "Procesing byte %d (out of %d)======> %d\n", j, n_bytes,me ) ;
          }
#endif
         size = size_pattern ;
          if ( sendcount - j < size_pattern )
          {
              size = sendcount  - j ;
          }

	 distance = levenshtein( pattern[i], &bufloc[j], size, column ) + size_pattern - size;

          if ( distance <= approx_factor ) {
             found++ ;
          }
      }
      matched=found;

    //reduce number of matched pattern of all processes into one sum.
    MPI_Reduce(&matched, &total, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
    if(me == 0) {
	  printf( "Number of matches for pattern <%s>: %d\n", pattern[i], total+n_matches[i] ) ;
	total = 0;
    }
  free( column );
  }

  /*****
   * END MAIN LOOP
   ******/
if(me==0){
  /* Timer stop */
  gettimeofday(&t2, NULL);
  duration = (t2.tv_sec -t1.tv_sec)+((t2.tv_usec-t1.tv_usec)/1e6);
  printf( "APM done in %lf s\n", duration ) ;
  free(buf);
}
  free(bufloc);
  MPI_Finalize();
  return 0 ;
}

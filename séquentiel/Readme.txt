Une petite modification a été réalisée au niveau du code séquentiel qui nous ait été fourni.
En effet, quand l'échantillon qui reste à étudier est inférieur à la taille du pattern, il 
faut considèrer ceci comme des lettres supprimées du pattern recherché 
ce qui fait que la distance soit égale à :
  distance = levenshtein( pattern[i], &buf[j], size, column ) + size_pattern - size;

avec size: la taille de l'échantillon qui reste à étudier.


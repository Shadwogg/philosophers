# philosophers
My version for the 42 project Philosophers.

NB : 

Les mutex doivent être initialisés après avoir initialisé la variable qu'elle protège, sinon fsanitize considère qu'il y a un data race.
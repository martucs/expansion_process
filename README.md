Break_down_str() function returns char ** of strings divided between "", '' and no quotes. WITH QUOTES INCLUDED

Example: for str = hola"que tal"'ak"aiu"jh'jkj it should return 

arr[0] = hola

arr[1] = "quetal"

arr[2] = 'ak"aiu"jh'

arr[3] = jkj

If it doesnt do this PLEASE LET ME KNOW

another_special_split() returns char **. it splits by  "" && '' AND REMOVES THEM

Example: for str = hola   "que  tal"'kajsh akjs "' it should return

arr[0] = hola  (with the spaces)

arr[1] = que tal

arr[2] = kajsh akjs "

checkout the my_special_split, it's like a split for spaces, it cuts the strings when it finds the spaces and skips and removes them EXCEPT if they are inside "" or ''

Example: for str =  hola "maj sh'  gq "kjhbla kjh, it should return 

arr[0] = hola

arr[1] = "maj sh'  gq "kjhbla

arr[2] = kjh

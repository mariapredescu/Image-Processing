PREDESCU Maria
GRUPA 334CB

	:::::::::  ::::::::::     :::     :::::::::  ::::    ::::  :::::::::: 
	:+:    :+: :+:          :+: :+:   :+:    :+: +:+:+: :+:+:+ :+:        
	+:+    +:+ +:+         +:+   +:+  +:+    +:+ +:+ +:+:+ +:+ +:+        
	+#++:++#:  +#++:++#   +#++:++#++: +#+    +:+ +#+  +:+  +#+ +#++:++#   
	+#+    +#+ +#+        +#+     +#+ +#+    +#+ +#+       +#+ +#+        
	#+#    #+# #+#        #+#     #+# #+#    #+# #+#       #+# #+#        
	###    ### ########## ###     ### #########  ###       ### ########## 


------------------------------------TEMA1 APD-----------------------------------

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>FUNCTIA RESIZE<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

Ininte de a incepe implementarea functiei de resize am realizat functia de citire,
respectiv scriere a unei imagini din/in fisier. Header-ul imaginii este format din
mai multe elemente(tipul imaginii - alb-negru sau color, dimensiunile imaginii,
maxvalue), acestea fiind urmate de matricea de pixeli a imaginii.
Initial am realizat implementarea fara thread-uri a acestei functii si am observat
ca pentru imaginii mai mari timpul de rulare este foarte mare. Astfel am paralelizat
procesul de resize cu ajutorul functiei threadFunction, unde se produce practic 
redimenionarea, iar in functia resize se creeaza si se executa thread-urile.

>>Implementare resize
Tinand cont de parametrii primiti ca input am impartit procesul de resize pe mai
ramuri in functie de numarul de culori din imagine(1 sau 3) si in functie de
paritatea factorului de redimensionare(acesta este fie par, fie 3).

1.Numarul de culori este 3(imaginea este color)
	a.Resize_factor este par
		-Matricea de pixeli este parcursa pe "patrate" de dimensiune resize_factor x
		resize_factor * numcolors
		-Se realizeaza suma pixelilor din fiecare patrat pentru fiecare culoare
		separat
		-Se completeaza matricea de pixeli rezultat cu media aritmetica a
		pixelilor din fiecare patrat pentru fiecare culoare(red, green, blue) separat
	b.Resize_factor este 3
		-Matricea de pixeli este parcursa pe "patrate" de dimensiune resize_factor x
		resize_factor * numcolors
		-Se realizeaza suma pixelilor din fiecare patrat inmultiti cu elementul
		corespunzator din matricea Gaussiana
		-Se completeaza matricea de pixeli rezultat cu suma obtinuta mai sus, 
		impartita la 16(suma elementelor din matricea Gaussiana) pentru fiecare
		culoare in parte
2.Numarul de culori este 1(imaginea este alb-negru)
	a.Resize_factor este par
		-Matricea este parcursa pe patrate resize_factor x resize_factor
		-Se realizeaza suma elementelor din fiecare patrat
		-Se completeaza matricea imaginii rezultat cu media aritmetica a 
		pixelilor din fiecare patrat
	b.Resize_factor este 3
		-Matricea este parcursa pe patrate resize_factor x resize_factor
		-Se realizeaza suma elementelor din fiecare patrat inmultite cu
		elementul corespunzator din matricea Gaussiana
		-Se completeaza matricea de pixeli rezultat cu suma obtinuta mai sus, 
		impartita la 16(suma elementelor din matricea Gaussiana)

Atunci cand dimensiunile matricei de pixeli nu se impart exact la resize_factor
si la numarul de thread-uri o sa existe anumite linii(am realizat paralelizarea 
pe linii) care nu o sa fie parcurse, iar astfel aceste linii vor ramane
necompletate in imaginea rezultat. Pentru aceasta, dupa fiecare din cazurile
prezentate mai sus am parcurs liniile ramase in acceasi maniera pentru a putea 
fi si ele completate in matricea de output.

Algoritmul prezentat mai sus a fost realizat in functia threadFunction, iar in
functia resize au fost creeate si executate thread-urile.
In urma rularilor repetate a programului cu numar diferit de thread-uri am
observat ca timpul de rulare scade odata cu marirea numarului de thread-uri.

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>FUNCTIA RENDER<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

Inainte de realizarea procesului de randare am completat functia de initializare
a imaginii in care am atribuit valori elementelor din header-ul imaginii(tipul
imaginii - alb-negru, dimensiunile imaginii - resolution x resolution si maxvalue
 - 255). Apoi am alocat memorie pentru matricea de pixeli a imaginii rezultate si
am initializat toti pixelii sa fie albi.
De asemenea, am realizat functia de scriere a unei imagini in fisier, la fel ca
la cerinta anterioara a temei.

>>Implementare render
Pentru a paraleliza procesul de randare am folosit functia threadFunction in care
se realizeaza practic randarea imaginii. 
Folosindu-ma de ecuatia dreptei oferita in enuntul temei (-x + 2 * y + 0 = 0) am
verificat daca un pixel se afla la o distanta mai mare de 3 cm de dreapta data 
cu ajutorul formulei matematice de determinare a distantei dintre un punct si
o dreapta: distance = abs(a * x + b * y + c) / sqrt(a^2 + b^2).
In cazul nostru  a = -1, b = 2, c = 0, iar  x si y sunt coordonatele pixelilor din
matricea imaginii. Pentru a determina aceasta distanta fata de mijlocul pixelului,
am inmultit fiecare coordonata cu dimensiunea unui pixel(100 / resolution) si am
adunat jumatate din dimensiune la final.
Dupa ce distanta a fost calculata s-a verificat daca aceasta este mai mica sau
egala cu 3. Daca conditia este indeplinita pixelul devine negru, altfel devine alb.

In functia render au fost create si executate thread-urile.
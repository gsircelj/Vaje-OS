#!/bin/bash

command=$1
shift

case $command in
help)
  echo "Za več podrobnosti dodajte kakšen ukaz! 
	Pravilna uporaba: 'bash project.sh help [ukaz]'"
;;
new)
  echo "Ustvari nov projekt s podanim imenom na trenutni lokaciji.
	V primeru podane poti, se projekt ustvari na podani poti. 

	Pravilna uporaba: 'bash project.sh new <ime projekta> [-d <pot>]'"
;;
activate)
  echo "Aktivira obstoječ projekt.

	Pravilna uporaba: 'bash project.sh activate [-d <pot>]'"
;;
add)
  echo "V projekt doda zbirko <ime>.c v src/ in <ime>.h v include/, po potrebi posodobi skripte za prevajanje.

	Pravilna uporaba: 'bash project.sh add <ime>'"
;;
build)
 echo "Aktivira obstoječ projekt. V okviru tega:

    nastavi samodopolnjevanje za ukaz project.sh (dopolnjevanje ukazov, imena testov),
    nastavi ustrezne okoljske spremenljivke za prevajanje in poganjanje projekta,
    spremeni PS1, tako da je v lupini sedaj vidno ime projekta v katerem se nahajamo
    požene novo lupino (bash)"
;;
test)
	echo "$ project.sh test [-lv] [test name]

Požene preizkuse programa.

Zastavica -l izpiše seznam imen preizkusov, ki se nahajajo v poti tests. Preizkusi so posamezne skripte, ki so poimenovane test_<ime testa>.sh. Ukaz izpiše le unikatna imena, brez test_ predpone in .sh končnice.

Zastavica -v požene preizkuse z ukazom valgrind.

Če podamo ime preizkusa (podamo lahko samo ime, brez predpone in končnice), se izvede le imenovani preizkusi. Drugače se izvedejo vsi preizkusi po abecednem redu.

Ukaz prešteje tudi število uspešno opravljenih preizkusov in to izpiše kot statistiko (število uspešnih in število vseh preizkusov)."
;;
run)
	echo "$ project.sh run

Požene program."
;;
"")
  echo "Vsi možni dokazi so: 
	New -
	Help - 
	Activate - 
	Add - 
	Build - 
	Run - 
	Test - "
;;
*)
    echo Napačen ukaz: $command
    exit -1
esac
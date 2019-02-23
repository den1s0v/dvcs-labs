# dvcs-labs
Practicing use of version control systems

Members: 

* Буланкин
* Головинова
* Денисов
* Донцов
* Ибрагим
* Калинкин
* Катышев
* Ковалев
* Курносов
* Курылев
* Махоткин
* Михайлов
* Поликарпова
* Рассадников

Git Bash Tips
=============

* Create branches command:
`for VAR in Bulankin  Golovinova  Denisov  Dontsov  Ibragim  Kalinkin  Katyshev  Kovalev  Kurnosov  Kurylev  Mahotkin  Mihailov  Polikarpova  Rassadnikov
do
git branch $VAR
done`


* Do not ask login & password every time you `git push`:
`git config --global credential.helper store`

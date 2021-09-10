# Triangle orientation histogram

OS Windows 10

MCVS 2015 v.14.0.25431.01 U3

Libs:

	SDL2-2.0.10
	SDL2_ttf-2.0.15

Build Win32
***
## En:

### Attention!

After transferring the project, you need to check and correct some of the project settings. In points 1 and 2, you must check/correct the paths to the suiting directories of the library. In points 3 and 4, you need to check that the content matches the lists below.

	Project -> Properties ->
	1. C/C++ -> General -> Aditional Include Directories:
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2-2.0.10\include
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2_ttf-2.0.15\include
	2. Linker -> General -> Aditional Library Directories:
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2-2.0.10\lib\x86
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2_ttf-2.0.15\lib\x86
	3. Linker -> Input -> Aditional Dependencies:
		SDL2.lib
		SDL2main.lib
		SDL2test.lib
		SDL2_ttf.lib
	4. Linker -> System -> SubSystem:
		Console (/SUBSYSTEM:CONSOLE)
***

### Program notes.

	1. The source (stl) files must be placed previously in the "Stl" directory, which is located in the same directory with the exe-file (to run the finished program), or in the source directory (to run the program from the studio).
	2. If the option to write results to file was specified in the action selection menu, it will be saved in the "Txt" directory.
	3. For view the results of previously made calculations, the file must be (previously saved) in the "Txt" directory.
	4. Only StL ASCII Format is recognized, while reading stl-file.
	5. If you delete the directory for txt files, then when you try to save the results, the program will report an error and continue working. But the results will not be saved.
	6. The configuration of the program settings is stored in the file ".\Resources\ChessLMV.ini".
		6.1. Section "workFolder", key "input" - directory for source (stl) files.
		6.2. Section "workFolder", key "output" - the directory where the calculation results (txt files) are saved and read from.
		6.3. Section "workWindow", the keys "iPositionX" и "iPositionY" - the coordinates of the upper left corner of the window in which the histogram is displayed.
		6.4. Section "workWindow", the keys "iWidth" и "iHeight" - the width and height (respectively) of the window in which the histogram is displayed.
		6.5. Independently of the section, the keys "isdlColorR", "isdlColorG", "isdlColorB", "isdlColorA" set the color of the suiting element.
		6.6. Section "histogramCadre", key "iDownField" - the distance from the bottom edge of the histogram window to the bottom edge of the frame of the histogram itself.
		6.7. Section "histogramCadre", key "iRightMinimalField" - minimum distance (may be greater) from the right edge of the histogram window to the right edge of the histogram frame itself.
		6.8. Section "histogramCadre", key "iScaleDivisionLen" - the length of the division on the histogram scale.
		6.9. Section "histogramCadre", the keys "iScaleValueIndentX" и "iScaleValueIndentY" - indent from the edge of the histogram frame to the signature near the vertical and horizontal scales, respectively.
		6.10. Section "font", key "strFont" - path to the file with the font.
		6.11. Section "font", key "iSize" - font size.
***

## Ru:

### Внимание!

После переноса проекта, необходимо проверить и поправить некоторые настройки проекта. В пунктах 1 и 2 необходимо указать/исправить пути к соответствующим каталогам билиотек. В пунктах 3 и 4 необходимо проверить, что содержимое соответствует спискам ниже.  

	Project -> Properties ->
	1. C/C++ -> General -> Aditional Include Directories:
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2-2.0.10\include
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2_ttf-2.0.15\include
	2. Linker -> General -> Aditional Library Directories:
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2-2.0.10\lib\x86
		Full_path_to_the_directory_with_the_project\TriangleOrientationHistogram\TriangleOrientationHistogram\Libs\SDL2_ttf-2.0.15\lib\x86
	3. Linker -> Input -> Aditional Dependencies:
		SDL2.lib
		SDL2main.lib
		SDL2test.lib
		SDL2_ttf.lib
	4. Linker -> System -> SubSystem:
		Console (/SUBSYSTEM:CONSOLE)
***

### Примечания по программе.

	1. Исходные (stl) файлы необходимо заранее поместить в каталог "Stl", которая находится в том же каталоге, что и exe-файл (для запуска готовой программы), или в каталоге с исходниками (для запуска программы из студии).
	2. Если в меню выбора действий был указан пункт записи результатов в файл, то он будет сохранён в каталоге "Txt".
	3. Для просмотра результатов ранее сделаных расчётов, файл должен находиться (ранее сохранён) в каталоге "Txt".
	4. При чтении stl-файла, распознаётся только StL ASCII Format.
	5. Если удалить каталог для txt-файлов, то при попытке сохранить результаты программа сообщит об ошибке и продолжит работать. Но результаты сохранены не будут.
	6. Конфигурация настроек программы хранится в файле ".\Resources\ChessLMV.ini".
		6.1. Секция "workFolder", ключ "input" - каталог, для исходных (stl) файлов.
		6.2. Секция "workFolder", ключ "output" - каталог, куда сохраняются и откуда считываются результаты расчётов (txt-файлы).
		6.3. Секция "workWindow", ключи "iPositionX" и "iPositionY" - координаты верхнего левого угла окна, в котором отображается гистограмма.
		6.4. Секция "workWindow", ключи "iWidth" и "iHeight" - ширина и высота (соответственно) окна, в котором отображается гистограмма.
		6.5. Независимо от секции, ключи "isdlColorR", "isdlColorG", "isdlColorB", "isdlColorA" задают цвет соответствующего элемента.
		6.6. Секция "histogramCadre", ключ "iDownField" - растояние от нижнего края окна гистограммы до нижнего края рамки самой гистограммы.
		6.7. Секция "histogramCadre", ключ "iRightMinimalField" - минимальное растояние (может быть больше) от правого края окна гистограммы до правого края рамки самой гистограммы.
		6.8. Секция "histogramCadre", ключ "iScaleDivisionLen" - длинна деления на шкале гистограммы.
		6.9. Секция "histogramCadre", ключи "iScaleValueIndentX" и "iScaleValueIndentY" - отступ от края рамки гистограммы до подписи возле вертикальной и горизонтальной шкалы соответственно.
		6.10. Секция "font", ключ "strFont" - путь к файлу со шрифтом.
		6.11. Секция "font", ключ "iSize" - размер шрифта.
***

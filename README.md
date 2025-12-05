# P2P шахматы на C++ 

Проект для игры в шашки по локальной сети. 
Реализован на языке С++ с использованием библиотеки SFML для графического интерфейса. Для взаимодействия используется модель Peer-to-peer.

## Как запустить?

### Готовый исполняемый файл
1. Скачайте архив с последним релизом из раздела [Releases](https://github.com/LaGGgggg/checkers/releases)
2. Запустите `main.exe` (Windows) или `main` (Linux/Mac OS)

### Самостоятельная сборка через [CMake](https://cmake.org/)

1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/LaGGgggg/checkers.git
   cd checkers
   ```

2. Настройте сборку:
   ```bash
   cmake -B build
   ```

3. Скомпилируйте проект:
    ```bash
    cmake --build build
    ```
    Для релизной сборки:
    ```bash
    cmake --build build --config Release
    ```

4. Запустите приложение:
    ```bash
    ./build/bin/main.exe  # Windows
    ./build/bin/main      # Linux/Mac OS
    ```

## Как играть?

### Настройка
Создайте файл `config.txt` в той же директории, что и `main.exe/main` и установите ip другого игрока в формате:
```txt
ip=141.33.4.12
```

### Настройки firewall для Windows

Для связи между двумя игроками необходимо добавить разрешение в firewall Windows:

Откройте powershell **от имени администратора** и выполните
(подставьте ip другого игрока и измените порты, если вы их меняли):
```powershell
New-NetFirewallRule `
  -DisplayName "Checkers" `
  -Direction Inbound `
  -Action Allow `
  -Protocol TCP `
  -LocalPort 9001,8012 `
  -RemoteAddress 141.33.4.12

New-NetFirewallRule `
  -DisplayName "Checkers" `
  -Direction Outbound `
  -Action Allow `
  -Protocol TCP `
  -LocalPort 9001,8012 `
  -RemoteAddress 141.33.4.12
```

*Для удаления всех установленных правил для приложения выполните:*
```powershell
Remove-NetFirewallRule -DisplayName "Checkers"
```

---
## Дополнительные возможности

### Смена шрифта
Шрифт находится в файле `font_data.cpp` в виде массива байтов.
Для генерации такого файла можно использовать `xxd` (подкомпонент `vim`):
```bash
xxd -i main_font.ttf > font_data.cpp
```

### Смена портов
По умолчанию используются порты `9001` и `8012`.
При необходимости можете изменить значения в `config.txt`:
```txt
port_1=9001
port_2=8012
```
Для корректной работы порты у обоих игроков должны совпадать.

---

## Для контрибьюторов

Правила:
- README.md пишем на русском языке
- Любая документация внутри кода пишется на английском языке
- Коммиты
  - Пишем на русском языке
  - Придерживаемся [Соглашения о коммитах](https://www.conventionalcommits.org/ru/v1.0.0/)
- Pull requests
  - Пишем на русском языке
- Ветки
  - `main` - основная ветка со стабильной версией;
    коммиты в эту ветку делаем только через pull requests из `main-<feature>`
  - `main-<feature>` - ветка для разработки конкретной функциональности
- В конце файлов оставляем одну пустую строку

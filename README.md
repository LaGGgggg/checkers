# P2P шахматы на C++ 

Peer-to-peer шахматы на C++ с использованием библиотеки SFML для графического интерфейса.

## Как запустить?

### Cmake

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

4. Запустите приложение:
    ```bash
    ./build/bin/Debug/main.exe  # Windows
    ./build/bin/Debug/main      # Linux/Mac OS
    ```

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

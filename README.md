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

4. Скопируйте файл `text.ttf` в директорию `build/bin`

5. Запустите приложение:
    ```bash
    ./build/bin/main.exe  # Windows
    ./build/bin/main      # Linux/Mac OS
    ```

## Как играть?

### Настройка

- Откройте файл `socket_manager.h` и установите ip другого игрока в константу `IP`:
    ```cpp
    ...
    const sf::IpAddress IP({ 141, 33, 4, 22 });  // пример для ip 141.33.4.12
    ...
    ```
    При необходимости можете изменить значения портов `port_1` и `port_2`.

### Windows

#### Настройки firewall

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

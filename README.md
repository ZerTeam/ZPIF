![Logo](images/ZPIF.png "Logo ZPIF")

**English (translation)**

# Description of the ZPIF File Format

ZPIF (ZerProjectImageFormat) is a raster format for storing graphical information.

## Format Structure

A ZPIF file consists of three main parts:

1. **Image Parameters**: Define the basic properties of the file.
2. **Commands**: Indicate the start of pixel data and the end of the file.
3. **Pixel Data Blocks**: Contain information about the pixels.

## File Header

The first line of the file contains the signature `DD 5A 50 49 46` in hex (binary representation) or the textual representation `�ZPIF`. This allows programs to identify the file as ZPIF.

## Image Parameters

Parameters are written within curly braces `{}`. Each parameter has a name and a value, where the value is enclosed in parentheses `()`. The parameter name defines its type, and the value specifies its content.

### Supported Parameters

#### Mandatory Parameters

- `{width} (1920)` — Image width in pixels.
- `{height} (1080)` — Image height in pixels.

#### Optional Parameters

- `{data} (2025-01-04)` — File creation date.
- `{time} (14:30:00)` — File creation time.
- `{unix_time} (1735691400)` — Date and time in UNIX format.
- `{utc} (-5)` — Time offset from Coordinated Universal Time (UTC).

## Commands

Commands are written in the format `\x00\x00\xXX\xXX\xXX\xXX` and define the structure of the image data:

- `\x00\x00\xFF\xFF\xFF\xFF\n` — Start of pixel data.
- `\x00\x00\x00\x00\x00\x00` — End of file.

## Pixel Data Blocks

Pixel data is written in consecutive blocks of 6 bytes: `\x00\x01\xFF\x00\x00\xFF`, where:

- The first two bytes are used for RLE (Run-Length Encoding) compression. They contain a number from 0 to 65535 in big-endian format. Note: If both bytes are 0, it is interpreted as a command.
- The remaining 4 bytes contain the pixel intensity in RGBA format.

Pixels are written from left to right, top to bottom.

## Writing Order

1. Image parameters are written in any order.
2. After the parameters, the command `\x00\x00\xFF\xFF\xFF\xFF\n` is written, indicating the start of pixel data.
3. At the end of the file, the command `\x00\x00\x00\x00\x00\x00` is written.

## Examples of ZPIF Files

### Example 1 (Binary Representation in Hex)

```
DD 5A 50 49 46 0A 7B 77 7D 28 32 29 0A 7B 28 32 29 0A 00 FF FF FF FF 0A 00 01 00 00 00 00 00 03 FF 00 00 FF 00 00 00 00 00 00
```

### Example 2 (Textual Representation, Null Bytes Replaced with `N`)

```
�ZPIF
{w}(2)
{h}(2)
NN����
NNNNNNNN�NN�NNNNNN
```

## Notes

1. Parameters can be abbreviated to their first letter (e.g., `{w}` instead of `{width}`).



**Русский (original)**

# Описание формата файлов ZPIF

ZPIF (ZerProjectImageFormat) — это растровый формат хранения графической информации.

## Структура формата

Файл ZPIF состоит из трёх основных частей:

1. **Параметры изображения**: задают основные свойства файла.
2. **Команды**: определяют начало данных пикселей и окончание файла.
3. **Блоки данных о пикселях**: содержат информацию о пикселях.

## Начало файла

Первая строка файла содержит сигнатуру `DD 5A 50 49 46` в hex (бинарном представлении) или текстовое представление `�ZPIF`. Это позволяет программам идентифицировать файл как ZPIF.

## Параметры изображения

Параметры записываются в фигурных скобках `{}`. Каждый параметр имеет имя и значение, где значение указывается в круглых скобках `()`. Имя параметра определяет его тип, а значение — содержание.

### Поддерживаемые параметры

#### Обязательные параметры

- `{width} (1920)` — Ширина изображения в пикселях.
- `{height} (1080)` — Высота изображения в пикселях.

#### Необязательные параметры

- `{data} (2025-01-04)` — Дата создания файла.
- `{time} (14:30:00)` — Время создания файла.
- `{unix_time} (1735691400)` — Дата и время в формате UNIX.
- `{utc} (-5)` — Смещение по времени относительно всемирного координированного времени (UTC).

## Команды

Команды записываются в формате `\x00\x00\xXX\xXX\xXX\xXX` и определяют структуру данных изображения:

- `\x00\x00\xFF\xFF\xFF\xFF\n` — Начало данных пикселей.
- `\x00\x00\x00\x00\x00\x00` — Конец файла.

## Блоки данных пикселей

Данные пикселей записываются в последовательных блоках по 6 байтов: `\x00\x01\xFF\x00\x00\xFF`, где:

- Первые два байта используются для сжатия RLE (Run-Length Encoding). В них записано число от 0 до 65535 в формате big-endian. Внимание: если оба байта равны 0, это интерпретируется как команда.
- Остальные 4 байта содержат интенсивность пикселя в формате RGBA.

Пиксели записываются слева направо, сверху вниз.

## Порядок записи

1. Параметры изображения записываются в произвольном порядке.
2. После параметров следует команда `\x00\x00\xFF\xFF\xFF\xFF\n`, указывающая на начало данных пикселей.
3. В конце файла записывается команда `\x00\x00\x00\x00\x00\x00`.

## Примеры файлов ZPIF

### Пример 1 (бинарное представление в hex)
```
DD 5A 50 49 46 0A 7B 77 7D 28 32 29 0A 7B 28 32 29 0A 00 FF FF FF FF 0A 00 01 00 00 00 00 00 03 FF 00 00 FF 00 00 00 00 00 00
```

### Пример 2 (текстовое представление, null-байты заменены на `N`)

```
�ZPIF
{w}(2)
{h}(2)
NN����
NNNNNNNN�NN�NNNNNN
```

## Примечания

1. Параметры могут быть сокращены до первой буквы (например, `{w}` вместо `{width}`).

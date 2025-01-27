![Logo](images/ZPIF.png "Logo ZPIF")

**English (translation)**

# ZPIF File Format Description

ZPIF (ZerProjectImageFormat) is a raster format for storing graphical information.

## Format Structure

A file consists of three main parts:

1. **Image Parameters**: Define the main properties of the file.
2. **Commands**: Specify the start of pixel data and the end of the file.
3. **Pixel Data**: Contain the colors of the pixels.

## File Start

The first line of the file contains `DD A5 50 49 46` in binary representation or `�ZPIF` in text. This indicates to programs that the file is in the ZPIF format.

## Image Parameters

Parameters are enclosed in curly braces `{}`. Each parameter has a name and a value. The value is enclosed in parentheses `()`, where the name defines the parameter type, and the value provides its content.

### Supported Parameters

#### Mandatory Parameters

- `{width} (1920)` - The width of the image in pixels.
- `{height} (1080)` - The height of the image in pixels.
- `{compression} (RLE)` - The compression method. If absent, the default value is `0`. Currently, only RLE is supported.

#### Optional Parameters

- `{data} (2025-01-04)` - The file creation date.
- `{time} (14:30:00)` - The file creation time.
- `{data_unix} (1735691400)` - The date and time in UNIX format.

## Commands

Commands are written in the format `@@` and define the structure of the image data:

- `@s@` - Marks the start of pixel data.
- `@e@` - Marks the end of the file.

## Recording Order

1. First, image parameters are recorded in any order.
2. After the parameters, the `@s@` command indicates the start of pixel data.
3. The file ends with the `@e@` command.

## Pixel Data

- Pixels are recorded in the format `[RGBA]` (each letter represents 1 byte of color), where the values represent the RGBA color intensities.
- When using RLE compression, parentheses `()` can precede the pixel description to specify the number of repeating pixels. Example: `(10)[\xFF\x00\x00\xFF]` means 10 consecutive red pixels.
- Pixels are recorded from left to right, top to bottom.

## Example 1 of a ZPIF File (Binary Representation)

```zpif
\xDDZPIF
{width} (4)
{height} (2)
{compression} (RLE)
{data} (2025-01-04)
{time} (14:30:00)
{data_unix} (1735691400)
@s@
(5) [\xFF\x80\x40\xFF]
[\xFF\xFF\x40\xFF]
(2) [\xFF\x80\x41\xFF]
@e@
```

## Example 2 of a ZPIF File (Text Representation)

```zpif
�ZPIF
{w} (2)
{h} (2)
{c} (0)
@s@
[���]
[����]
[��1�]
[��1�]
@e@
```

## Notes

1. Parameters can be abbreviated to their first letter (except for `data_unix`, whose abbreviation is `u`).



**Русский (original)**

# Описание формата файлов ZPIF

ZPIF (ZerProjectImageFormat) - это растровый формат хранения графической информации.

## Структура формата

Файл состоит из трёх основных частей:

1. **Параметры изображения**: задают основные свойства файла.
2. **Команды**: определяют начало данных пикселей и окончание файла.
3. **Данные о пикселях**: данные, хранящие цвета пикселей.

## Начало файла

Первая строка файла содержит `DD A5 50 49 46` в бинарном представлении или же `�ZPIF` в текстовом. Это говорит программам, что это файл в формате ZPIF.

## Параметры изображения

Параметры записываются в фигурных скобках `{}`. Каждый параметр имеет имя и значение. Значение записывается в круглых скобках `()`, где имя задаёт тип параметра, а значение — его содержание. 

### Поддерживаются следующие параметры

#### Обязательные параметры

- `{width} (1920)` - Ширина изображения в пикселях.
- `{height} (1080)` - Высота изображения в пикселях.
- `{compression} (RLE)` - Метод сжатия. Если отсутствует, значение по умолчанию — `0`. Поддерживается пока только RLE.

#### Необязательные параметры

- `{data} (2025-01-04)` - Дата создания файла.
- `{time} (14:30:00)` - Время создания файла.
- `{data_unix} (1735691400)` - Дата и время в формате UNIX.

## Команды

Команды записываются в формате `@@` и определяют структуру данных изображения:

- `@s@` - Определяет начало данных пикселей.
- `@e@` - Определяет конец файла.

## Порядок записи

1. Сначала записываются параметры изображения в произвольном порядке.
2. После параметров идёт команда `@s@`, указывающая на начало данных пикселей.
3. В конце файла записывается команда `@e@`.

## Данные пикселей

- Пиксели записываются в формате `[RGBA]` (каждая буква — это 1 байт цвета), где значения представляют интенсивности цветов RGBA.
- При использовании сжатия RLE перед описанием пикселя могут использоваться круглые скобки `()`, содержащие количество повторяющихся пикселей. Пример: `(10)[\xFF\x00\x00\xFF]` означает 10 красных пикселей подряд.
- Пиксели записываются слева направо, сверху вниз.

## Пример 1 файла ZPIF (в бинарном представлении)

```zpif
\xDDZPIF
{width} (4)
{height} (2)
{compression} (RLE)
{data} (2025-01-04)
{time} (14:30:00)
{data_unix} (1735691400)
@s@
(5) [\xFF\x80\x40\xFF]
[\xFF\xFF\x40\xFF]
(2) [\xFF\x80\x41\xFF]
@e@
```

## Пример 2 файла ZPIF (в текстовом представлении)

```zpif
�ZPIF
{w} (2)
{h} (2)
{c} (0)
@s@
[���]
[����]
[��1�]
[��1�]
@e@
```

## Примечания

1. У параметров есть сокращения до одной первой буквы (кроме `data_unix`, его сокращение — `u`).

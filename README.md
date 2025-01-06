![Logo](images/ZPIF.png "Logo ZPIF")

**English (translation)**

# ZPIF File Format Description

ZPIF (ZerProjectImageFormat) is a raster format for storing graphical data.

## Format Structure

The file consists of two main parts:

1. **Image Parameters**: Define the main properties of the file.
2. **Commands**: Indicate the start of pixel data and the end of the file.

## Image Parameters

Parameters are enclosed in curly braces `{}`. Each parameter has a name and a value. The value is specified in parentheses `()`, where the name indicates the parameter type, and the value contains its content.

### Supported Parameters

#### Mandatory Parameters:

- `{format} (ZPIFv1)` - File format. Specifies the type of image.
- `{mode} (RGB)` - Display mode. Currently, only RGB is supported.
- `{width} (1920)` - Image width in pixels.
- `{height} (1080)` - Image height in pixels.
- `{compression} (RLE)` - Compression method. If absent, the default value is `0`. Currently, only RLE is supported.

#### Optional Parameters:

- `{data} (2025-01-04)` - File creation date.
- `{time} (14:30:00)` - File creation time.
- `{data_unix} (1735691400)` - Date and time in UNIX format.

## Commands

Commands are written using the `@@` format and define the structure of image data:

- `@start@` - Marks the beginning of pixel data.
- `@end@` - Marks the end of the file.

## Writing Order

1. Image parameters are written first in any order.
2. The `@start@` command follows, indicating the start of pixel data.
3. The file ends with the `@end@` command.

## Pixel Data

- Pixels are recorded in the `[RGB]` format (each letter represents 1 byte of color), where values indicate the intensity of RGB colors.
- When using RLE compression, parentheses `()` may precede a pixel description, specifying the number of repeating pixels. Example: `(10)[255 0 0]` means 10 consecutive red pixels.
- Pixels are written from left to right, top to bottom.

## Example 1: ZPIF File (Binary Representation)

```zpif
{format} (zpifv1)
{mode} (rgb)
{width} (4)
{height} (2)
{compression} (RLE)
{data} (2025-01-04)
{time} (14:30:00)
{data_unix} (1735691400)
@start@
(5) [\xFF\x80\x40]
[\xFF\xFF\x40]
(2) [\xFF\x80\x41]
@end@
```

## Example 2: ZPIF File (Textual representation)

```zpif
{f} (zpifv1)
{m} (rgb)
{w} (2)
{h} (2)
{c} (0)
@s@
[���]
[���]
[��1]
[�<1]
@e@
```

## Notes

1. Parameters and commands have single-letter abbreviations (except for `data_unix`, which is abbreviated as `u`).


**Русский (original)**

# Описание формата файла ZPIF

ZPIF (ZerProjectImageFormat) - это растровый формат хранения графической информации.

## Структура формата

Файл состоит из двух основных частей:

1. **Параметры изображения**: задают основные свойства файла.
2. **Команды**: определяют начало данных пикселей и окончание файла.

## Параметры изображения

Параметры записываются в фигурных скобках `{}`. Каждый параметр имеет имя и значение. Значение записывается в круглых скобках `()`, где имя задаёт тип параметра, а значение его содержание. 

### Поддерживаются следующие параметры

#### Обязательные параметры:

- `{format} (ZPIFv1)` - Формат файла. Указывает тип изображения.
- `{mode} (RGB)` - Режим отображения. Поддерживается пока только RGB.
- `{width} (1920)` - Ширина изображения в пикселях.
- `{height} (1080)` - Высота изображения в пикселях.
- `{compression} (RLE)` - Метод сжатия. Если отсутствует, значение по умолчанию — `0`. Поддерживается пока только RLE.

#### Необязательные параметры:

- `{data} (2025-01-04)` - Дата создания файла.
- `{time} (14:30:00)` - Время создания файла.
- `{data_unix} (1735691400)` - Дата и время в формате UNIX.

## Команды

Команды записываются в формате `@@` и определяют структуру данных изображения:

- `@start@` - Определяет начало данных пикселей.
- `@end@` - Определяет конец файла.

## Порядок записи

1. Сначала записываются параметры изображения в произвольном порядке.
2. После параметров идёт команда `@start@`, указывающая на начало данных пикселей.
3. В конце файла записывается команда `@end@`.

## Данные пикселей

- Пиксели записываются в формате `[RGB]`(каждая буква это 1 байт цвета), где значения представляют интенсивности цветов RGB.
- При использовании сжатия RLE перед описанием пикселя могут использоваться круглые скобки `()`, содержащие количество повторяющихся пикселей. Пример: `(10)[255 0 0]` означает 10 красных пикселей подряд.
- Пиксели записываются слева направо, сверху вниз.

## Пример 1 файла ZPIF(в бинарном предстовлении):

```zpif
{format} (zpifv1)
{mode} (rgb)
{width} (4)
{height} (2)
{compression} (RLE)
{data} (2025-01-04)
{time} (14:30:00)
{data_unix} (1735691400)
@start@
(5) [\xFF\x80\x40]
[\xFF\xFF\x40]
(2) [\xFF\x80\x41]
@end@
```

## Пример 2 файла ZPIF(в текстовом представлении):

```zpif
{f} (zpifv1)
{m} (rgb)
{w} (2)
{h} (2)
{c} (0)
@s@
[���]
[���]
[��1]
[�<1]
@e@
```

## Примечания

1. У параметров и команд есть сокращения до одной первой буквы (кроме `data_unix`, его сокращение — `u`).

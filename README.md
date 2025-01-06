![Logo](images/ZPIF.png "Logo ZPIF")

# English (translation)

# ZPIF File Format Description

ZPIF (ZerProjectImageFormat) is a raster format for storing graphic information.

## Format Structure

The file consists of two main sections:

1. **Image Parameters**: Define the basic properties of the file.
2. **Commands**: Mark the start of pixel data and the end of the file.

## Image Parameters

Parameters are enclosed in curly braces `{}`. Each parameter has a name and a value. The value is specified in parentheses `()`, where the name indicates the parameter type, and the value defines its content.

### Supported Parameters

#### Mandatory Parameters:

- `{format} (ZPIFv1)` - File format. Specifies the image type.
- `{mode} (RGB)` - Display mode. Currently, only RGB is supported.
- `{width} (1920)` - Image width in pixels.
- `{height} (1080)` - Image height in pixels.
- `{compression} (RLE)` - Compression method. If omitted, the default value is `0`. Currently, only RLE is supported.

#### Optional Parameters:

- `{data} (2025-01-04)` - File creation date.
- `{time} (14:30:00)` - File creation time.
- `{data_unix} (1735691400)` - Date and time in UNIX format.

## Commands

Commands are written in the `@@` format and define the image data structure:

- `@start@` - Marks the beginning of pixel data.
- `@end@` - Marks the end of the file.

## Recording Order

1. Image parameters are recorded first, in any order.
2. The `@start@` command follows, indicating the beginning of pixel data.
3. The file ends with the `@end@` command.

## Pixel Data

- Pixels are recorded in the format `[0-255 0-255 0-255]`, where the values represent the RGB color intensities.
- When using RLE compression, parentheses `()` may precede pixel descriptions, indicating the number of repeated pixels. For example, `(10)[255 0 0]` represents 10 consecutive red pixels.
- Pixels are recorded from left to right, top to bottom.

## Example 1 of a ZPIF File:

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
(5) [255 0 0]
[0 255 0]
(2) [0 0 255]
@end@
```

## Example 2 of a ZPIF File:

```zpif
{f} (zpifv1)
{m} (rgb)
{w} (2)
{h} (2)
{c} (0)
@s@
[255 0 0]
[255 0 0]
[0 255 0]
[0 0 255]
@e@
```

## Notes

1. Parameters and commands can be abbreviated to their first letter (except for `data_unix`, which is abbreviated as `u`).

# Русский (original)

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

- Пиксели записываются в формате `[0-255 0-255 0-255]`, где значения представляют интенсивности цветов RGB.
- При использовании сжатия RLE перед описанием пикселя могут использоваться круглые скобки `()`, содержащие количество повторяющихся пикселей. Пример: `(10)[255 0 0]` означает 10 красных пикселей подряд.
- Пиксели записываются слева направо, сверху вниз.

## Пример 1 файла ZPIF:

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
(5) [255 0 0]
[0 255 0]
(2) [0 0 255]
@end@
```

## Пример 2 файла ZPIF:

```zpif
{f} (zpifv1)
{m} (rgb)
{w} (2)
{h} (2)
{c} (0)
@s@
[255 0 0]
[255 0 0]
[0 255 0]
[0 0 255]
@e@
```

## Примечания

1. У параметров и команд есть сокращения до одной первой буквы (кроме `data_unix`, его сокращение — `u`).

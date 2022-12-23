# Procedural-game-map-generation #
Данная работа выполнена в качестве практики во 2-м семестре 3-го курса программы ПМИ в РТУ МИРЭА.

## Постановка задачи ##
Разработать модель карты города и реализовать её процедурную генерацию для каждой новой игровой сессии.

Карта должна состоять из параллельных пластов прямоугольных районов (горизонтальные линии районов должны быть параллельны друг другу), в которых есть по несколько домов. От каждой двери дома провести тропинку, идущую к более крупной дороге, окружающей район. Размеры карты, домов, дорог, а также количество районов и домов в них реализовать в виде входных параметров, от которых также зависит генерация.

## Модель карты ##
Общая структура карты города будет выглядеть следующим образом (красным выделены двери домов):
<p align="center">
    <img src="images/General_structure_of_the_city_map.png" width = "500">
</p>

Рисоваться карта будет в текстовом редакторе "Блокнот" и будут использованы следующие обозначения:
* "*" - границы города;
* "H" - дороги (и тропинки, и основные дороги);
* "D" - дверь дома;
* "1" - границы дома;
* "2" - внутренняя часть дома;
* "0" - пустое пространство на карте.

## Параметры модели ##

В качестве параметров для генерации используем:
* height - высота карты в количестве символов без учета границы и дорог;
* heightCount - количество районов в высоту;
* width - ширина карты в количестве символов без учета границы и дорог;
* wideCount - количество районов в ширину;
* roadSize - ширина основных дорог в количестве символов (для тропинок ширина постоянная и равна 1));
* diffPercent - коэффициент искажения размера района относительно среднего размера, вычисленного по формуле $\frac{height}{heightCount}$ или $\frac{width}{wideCount}$ в зависимости от расположения (от 0 до 1);
* housePercent - зависимость количества домов в районе от его размеров (от 0 до 1).

## Реализация ##

Всю информацию по постановке задачи, алгоритму и реализации процедурной генерации карты города можно найти в [отчёте по практике](https://github.com/thecrazymage/Procedural-game-map-generation/blob/main/%D0%9F%D1%80%D0%B0%D0%BA%D1%82%D0%B8%D0%BA%D0%B0.pdf).

## Результаты ##

Три разные реализация модели для параметров height = 45, width = 60, wideCount = 3, heightCount = 3, diffPercent = 0.25, roadSize = 2, housePercent = 0.33 выглядят так:  
1. 
  <p align="center">
      <img src="images/Example1.png" width=400>
  </p>
2. 
  <p align="center">
      <img src="images/Example2.png" width=400>
  </p>
3. 
  <p align="center">
      <img src="images/Example3.png" width=400>
  </p>

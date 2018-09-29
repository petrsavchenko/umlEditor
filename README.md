# UML editor 

UML editor made in [Qt](https://www.qt.io) which allows to create diagrams: Class diagram, Use case diagram and State diagram, setting relationship between diagram elements, and save diagram to file. 

The project was part of my course work on the 2nd year studying in university. So original plan was to support other types of UML diagrams and implement AI to analyse used diagrams to make suggestion to improve them and generate source code in selected programming language.

## Folders structure

```
my_diagram_arrow.cpp
my_diagram_arrow.h
my_diagram_item.cpp
my_diagram_item.h
my_diagram_path_item.cpp
my_diagram_path_item.h
my_diagram_scene.cpp
my_diagram_scene.h
my_diagram_text_item.cpp
my_diagram_text_item.h
...
mainwindow.h
mainwindow.cpp
```

`my_diagram_scene` is view for diagram elements. Inherites from QGraphicsScene. It stores mode of current diagram, encapsulates methods of draw, positioning and setting fonts in items.</br>
`my_diagram_arrow` is relationship between elements. E.g. in case of class diagram it is Dependency, Assosiation, Aggregation or Composition.</br>
`my_diagram_item` is diagram element. It has type and properties. It also stores relationships with other elements.</br>
`my_diagram_text_item` is property of diagram element. It contains color, text, font of text and so on.

## Authors

[**Petr Savchenko**](http://petrsavchenko.ru) - retarded full stack dev from Russia :snowflake:
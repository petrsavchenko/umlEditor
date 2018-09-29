#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "my_diagram_scene.h"
#include "my_diagram_arrow.h"
#include "my_diagram_item.h"
#include "my_diagram_text_item.h"

#include"QToolButton"
#include"QAbstractButton"
#include"QLabel"
#include"QSplitter"
#include"QMessageBox"
#include"QString"
#include"QFileDialog"
#include "QList"
#include "QTextDocument"
#include "QCloseEvent"
#include "QProcess"
const int InserttextButton = 20; // константа говорит что мы будем вводить текст в диаграмму

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCurrentFile("");
    QSplitter * Spl = new QSplitter (Qt::Horizontal);
    Spl->addWidget(ui->tabWidget);
    Spl->addWidget(ui->graphicsView);
    ui->gridLayout->addWidget(Spl);
    create_font_text_scale();
//++++Работа со сценой++++
    itemMenu = new QMenu();
    itemMenu->addAction(ui->On_delete);
    itemMenu->addSeparator();
    itemMenu->addAction(ui->On_front);
    itemMenu->addAction(ui->On_back);
    scene = new My_diagram_scene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(My_diagram_item*)),this, SLOT(itemInserted(My_diagram_item*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),this, SLOT(itemSelected(QGraphicsItem*)));
    connect(ui->menu_2,SIGNAL(aboutToShow()),this,SLOT(help()));
    ui->graphicsView->setScene(scene);
    create_tool_bars();
    readSettings();
    elms_on_scene = -1;
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}
//+++++++++++++++++++++++++++++Функции для редактирования текста+++++++++++++++++++++++++++++++++++
void MainWindow::fontSizeChanged(const QString &size)
{
    handleFontChange();
}
void MainWindow::currentFontChanged(const QFont &font)
{
    handleFontChange();
}
void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(ui->Bold->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(ui->Italic->isChecked());
    font.setUnderline(ui->Underline->isChecked());
    scene->setFont(font);
}
void MainWindow::create_tool_bars()
{
    //==================================================
    //
    ////==================================================
    //
    ////==================================================
    //
    //

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(QPixmap(":/move.png").scaled(47,47)));
    pointerButton->setToolTip("Простое перемещение объектов");
    pointerButton->setIconSize(QSize(50,50));

    QToolButton *linePointerButton2 = new QToolButton;
    linePointerButton2->setCheckable(true);
    linePointerButton2->setIcon(QIcon(QPixmap(":/dependency-right.png").scaled(47,47)));
    linePointerButton2->setToolTip("Зависимость");
    linePointerButton2->setIconSize(QSize(50,50));

    QToolButton *linePointerButton3 = new QToolButton;
    linePointerButton3->setCheckable(true);
    linePointerButton3->setIcon(QIcon(QPixmap(":/association-right.png").scaled(47,47)));
    linePointerButton3->setToolTip("Ассоциация");
    linePointerButton3->setIconSize(QSize(50,50));

    QToolButton *linePointerButton4 = new QToolButton;
    linePointerButton4->setCheckable(true);
    linePointerButton4->setIcon(QIcon(QPixmap(":/aggregation-left.png").scaled(47,47)));
    linePointerButton4->setToolTip("Агрегация");
    linePointerButton4->setIconSize(QSize(50,50));

    QToolButton *linePointerButton5 = new QToolButton;
    linePointerButton5->setCheckable(true);
    linePointerButton5->setIcon(QIcon(QPixmap(":/composition-left.png").scaled(47,47)));
    linePointerButton5->setToolTip("Композиция");
    linePointerButton5->setIconSize(QSize(50,50));

    QToolButton *linePointerButton6 = new QToolButton;
    linePointerButton6->setCheckable(true);
    linePointerButton6->setIcon(QIcon(QPixmap(":/generalization.png").scaled(47,47)));
    linePointerButton6->setToolTip("Обобщение");
    linePointerButton6->setIconSize(QSize(50,50));

    // Зависимость (линия пунктир на конце стрелка),
    // Ассоциация (Сплошная линия),
    // Агрегация (Линия с пустым ромбом, который находится ближе к классу который является целым,
    //           т.е. который вмещает в себя все те агригуемые части)
    // Композиция (частный случай агрегации только его части не являются самостоятельными программами ЗАКРАШЕННЫЙ РОМБ)
    // Обобщение Сплошная линия с Пустой стрелкой

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(My_diagram_scene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton2, int(My_diagram_scene::Dependency));
    pointerTypeGroup->addButton(linePointerButton3, int(My_diagram_scene::Association));
    pointerTypeGroup->addButton(linePointerButton4, int(My_diagram_scene::Aggregation));
    pointerTypeGroup->addButton(linePointerButton5, int(My_diagram_scene::Composition));
    pointerTypeGroup->addButton(linePointerButton6, int(My_diagram_scene::Generalization));

    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),this, SLOT(pointerGroupClicked(int)));
    //======================================================сделали режимы сцены
    buttonGroup = new QButtonGroup(this);       // мы создаем новый контейнер
    buttonGroup->setExclusive(false);           //Может быть нажата только одна кнопка из этой группы
    connect(buttonGroup, SIGNAL(buttonClicked(int)),this, SLOT(buttonGroupClicked(int)));

    QGridLayout *layout = new QGridLayout;
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line,1,0,2,0);

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    textButton->setIcon(QIcon(QPixmap(":/text.png").scaled(47, 47)));
    textButton->setIconSize(QSize(50, 50));
    buttonGroup->addButton(textButton, InserttextButton);

    QGridLayout *textLayout = new QGridLayout();
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel("Текст"), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);

    QGridLayout *arrowLayout = new QGridLayout();
    arrowLayout->addWidget(pointerButton, 0, 0, Qt::AlignHCenter);
    arrowLayout->addWidget(new QLabel("Перемещение"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget = new QWidget;
    arrowWidget->setLayout(arrowLayout);
    layout->addWidget(arrowWidget,2,0);

    QGridLayout *arrowLayout2 = new QGridLayout();
    arrowLayout2->addWidget(linePointerButton2, 0, 0, Qt::AlignHCenter);
    arrowLayout2->addWidget(new QLabel("Зависимость"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget2 = new QWidget;
    arrowWidget2->setLayout(arrowLayout2);
    layout->addWidget(arrowWidget2,2,1);

    QGridLayout *arrowLayout3 = new QGridLayout();
    arrowLayout3->addWidget(linePointerButton3, 0, 0, Qt::AlignHCenter);
    arrowLayout3->addWidget(new QLabel("Ассоциация"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget3 = new QWidget;
    arrowWidget3->setLayout(arrowLayout3);
    layout->addWidget(arrowWidget3,3,0);

    QGridLayout *arrowLayout4 = new QGridLayout();
    arrowLayout4->addWidget(linePointerButton4, 0, 0, Qt::AlignHCenter);
    arrowLayout4->addWidget(new QLabel("Агрегация"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget4 = new QWidget;
    arrowWidget4->setLayout(arrowLayout4);
    layout->addWidget(arrowWidget4,3,1);

    QGridLayout *arrowLayout5 = new QGridLayout();
    arrowLayout5->addWidget(linePointerButton5, 0, 0, Qt::AlignHCenter);
    arrowLayout5->addWidget(new QLabel("Композиция"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget5 = new QWidget;
    arrowWidget5->setLayout(arrowLayout5);
    layout->addWidget(arrowWidget5,4,0);

    QGridLayout *arrowLayout6 = new QGridLayout();
    arrowLayout6->addWidget(linePointerButton6, 0, 0, Qt::AlignHCenter);
    arrowLayout6->addWidget(new QLabel("Обобщение"), 1, 0, Qt::AlignCenter);
    QWidget *arrowWidget6 = new QWidget;
    arrowWidget6->setLayout(arrowLayout6);
    layout->addWidget(arrowWidget6,4,1);

    layout->addWidget(createCellWidget("Класс",My_diagram_item::Class_),0,0);
    layout->addWidget(textWidget, 0, 1);
    layout->addWidget(createCellWidget("Интерфейс",My_diagram_item::Interface),1,0);


    layout->setRowStretch(5,10);
    ui->elems->setLayout(layout);
//==================================================
//
//
    //==================================================
    //
    //
    //==================================================
    //
    //
     QGridLayout *layout2 = new QGridLayout;
     layout2->addWidget(createCellWidget("Актер",My_diagram_item::Actor),0,0);
     layout2->addWidget(createCellWidget("Вариант использования",My_diagram_item::Option),0,1);

     QToolButton *one_button_ = new QToolButton;
     one_button_->setCheckable(true);
     one_button_->setChecked(true);
     one_button_->setIcon(QIcon(QPixmap(":/move.png").scaled(47,47)));
     one_button_->setToolTip("Простое перемещение объектов");
     one_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout = new QGridLayout();
     tab2layout->addWidget(one_button_, 0, 0, Qt::AlignHCenter);
     tab2layout->addWidget(new QLabel("Перемещение"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget = new QWidget;
     tabWidget->setLayout(tab2layout);

     QToolButton *two_button_ = new QToolButton;
     two_button_->setCheckable(true);
     two_button_->setIcon(QIcon(QPixmap(":/generalization.png").scaled(47,47)));
     two_button_->setToolTip("Обобщение");
     two_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout2 = new QGridLayout();
     tab2layout2->addWidget(two_button_, 0, 0, Qt::AlignHCenter);
     tab2layout2->addWidget(new QLabel("Обобщение"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget2 = new QWidget;
     tabWidget2->setLayout(tab2layout2);

     QToolButton *three_button_ = new QToolButton;
     three_button_->setCheckable(true);
     three_button_->setIcon(QIcon(QPixmap(":/dependency-right.png").scaled(47,47)));
     three_button_->setToolTip("Зависимость");
     three_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout3 = new QGridLayout();
     tab2layout3->addWidget(three_button_, 0, 0, Qt::AlignHCenter);
     tab2layout3->addWidget(new QLabel("Зависимость"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget3 = new QWidget;
     tabWidget3->setLayout(tab2layout3);

     QToolButton *four_button_ = new QToolButton;
     four_button_->setCheckable(true);
     four_button_->setIcon(QIcon(QPixmap(":/include.png").scaled(47,47)));
     four_button_->setToolTip("Включение");
     four_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout4 = new QGridLayout();
     tab2layout4->addWidget(four_button_, 0, 0, Qt::AlignHCenter);
     tab2layout4->addWidget(new QLabel("Включение"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget4 = new QWidget;
     tabWidget4->setLayout(tab2layout4);

     QToolButton *five_button_ = new QToolButton;
     five_button_->setCheckable(true);
     five_button_->setIcon(QIcon(QPixmap(":/extend.png").scaled(47,47)));
     five_button_->setToolTip("Расширение");
     five_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout5 = new QGridLayout();
     tab2layout5->addWidget(five_button_, 0, 0, Qt::AlignHCenter);
     tab2layout5->addWidget(new QLabel("Расширение"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget5 = new QWidget;
     tabWidget5->setLayout(tab2layout5);

     QToolButton *six_button_ = new QToolButton;
     six_button_->setCheckable(true);
     six_button_->setIcon(QIcon(QPixmap(":/association-right.png").scaled(47,47)));
     six_button_->setToolTip("Ассоциация");
     six_button_->setIconSize(QSize(50,50));

     QGridLayout *tab2layout6 = new QGridLayout();
     tab2layout6->addWidget(six_button_, 0, 0, Qt::AlignHCenter);
     tab2layout6->addWidget(new QLabel("Ассоциация"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget6 = new QWidget;
     tabWidget6->setLayout(tab2layout6);

     tab_2_Group = new QButtonGroup(this);
     tab_2_Group->addButton(one_button_, int(My_diagram_scene::MoveItem));
     tab_2_Group->addButton(two_button_, int(My_diagram_scene::Generalization));
     tab_2_Group->addButton(three_button_, int(My_diagram_scene::Dependency));
     tab_2_Group->addButton(four_button_, int(My_diagram_scene::Include));
     tab_2_Group->addButton(five_button_, int(My_diagram_scene::Extend));
     tab_2_Group->addButton(six_button_, int(My_diagram_scene::Association));

     connect(tab_2_Group, SIGNAL(buttonClicked(int)),this, SLOT(tab_2_GroupClicked(int)));

     layout2->addWidget(tabWidget,1,0);
     layout2->addWidget(tabWidget2,1,1);
     layout2->addWidget(tabWidget3,2,0);
     layout2->addWidget(tabWidget4,2,1);
     layout2->addWidget(tabWidget5,3,0);
     layout2->addWidget(tabWidget6,3,1);
     //     QSplitter * Spl = new QSplitter (Qt::Vertical);
     //     Spl->addWidget(tabWidget);
     //     Spl->addWidget(tabWidget2);
     //     layout2->addWidget(Spl);

     layout2->setRowStretch(4,10);
     ui->use_case->setLayout(layout2);
     //==================================================
     //
     //
         //==================================================
         //
         //
         //==================================================
         //
         //
     QGridLayout *layout3 = new QGridLayout;
     // layout2->addWidget(createCellWidget("Актер",My_diagram_path_item::Actor),0,0);


     QToolButton *one_button_state = new QToolButton;
     one_button_state->setCheckable(true);
     one_button_state->setChecked(true);
     one_button_state->setIcon(QIcon(QPixmap(":/move.png").scaled(47,47)));
     one_button_state->setToolTip("Простое перемещение объектов");
     one_button_state->setIconSize(QSize(50,50));

     QGridLayout *tab2layout_state = new QGridLayout();
     tab2layout_state->addWidget(one_button_state, 0, 0, Qt::AlignHCenter);
     tab2layout_state->addWidget(new QLabel("Перемещение"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget_state = new QWidget;
     tabWidget_state->setLayout(tab2layout_state);

     QToolButton *six_button_state = new QToolButton;
     six_button_state->setCheckable(true);
     six_button_state->setIcon(QIcon(QPixmap(":/association-right.png").scaled(47,47)));
     six_button_state->setToolTip("Транзакция");
     six_button_state->setIconSize(QSize(50,50));

     QGridLayout *tab2layout_state_2 = new QGridLayout();
     tab2layout_state_2->addWidget(six_button_state, 0, 0, Qt::AlignHCenter);
     tab2layout_state_2->addWidget(new QLabel("Переход"), 1, 0, Qt::AlignCenter);
     QWidget *tabWidget_state_2 = new QWidget;
     tabWidget_state_2->setLayout(tab2layout_state_2);

     State_Group = new QButtonGroup(this);
     State_Group->addButton(one_button_state,int(My_diagram_scene::MoveItem));
     State_Group->addButton(six_button_state,int(My_diagram_scene::Transaction));
     connect(State_Group, SIGNAL(buttonClicked(int)),this, SLOT(State_GroupClicked(int)));

     layout3->setRowStretch(5,10);

     layout3->addWidget(createCellWidget("Простое состояние",My_diagram_item::State),0,0);
     layout3->addWidget(createCellWidget("Начальное состояние",My_diagram_item::BeginState),0,1);
     layout3->addWidget(createCellWidget("Конечное состояние",My_diagram_item::EndState),1,0);
     layout3->addWidget(createCellWidget("Состояние синхронизации",My_diagram_item::SyncState),1,1);
     layout3->addWidget(createCellWidget("Недавнее\nисторическое\nсостояние",My_diagram_item::Shallow_history_state),2,0);
     layout3->addWidget(createCellWidget("Давнее\nисторическое\nсостояние",My_diagram_item::Deep_history_state),2,1);
     layout3->addWidget(createCellWidget("Параллельный переход",My_diagram_item::Transition),3,0);

     layout3->addWidget(tabWidget_state);
     layout3->addWidget(tabWidget_state_2);
     ui->automation->setLayout(layout3);

}
//for constructor
void MainWindow::create_font_text_scale()
{
    //++++++++++++++++++++++++Инициализация_Шрифта+++++++++++++++++++++++++++++++++++++
        fontCombo = new QFontComboBox();
        connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
                this, SLOT(currentFontChanged(QFont)));
    //++++++++++++++++++++++++Инициализация_Размера_Шрифта+++++++++++++++++++++++++++++++++++++
        fontSizeCombo = new QComboBox;
        fontSizeCombo->setEditable(true);
        for (int i = 8; i < 30; i = i + 2)
            fontSizeCombo->addItem(QString().setNum(i));
        QIntValidator *validator = new QIntValidator(2, 64, this);
        fontSizeCombo->setValidator(validator);
        ui->Text_ToolBar->addWidget(fontCombo);
        ui->Text_ToolBar->addWidget(fontSizeCombo);
        connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(fontSizeChanged(QString)));
    //++++++++++++++++++++++++Инициализация_Масштаба+++++++++++++++++++++++++++++++++++++
        sceneScaleCombo = new QComboBox;
       // sceneScaleCombo->setEditable(true);
        QStringList scales;
        scales << "10%" << "25%" << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
        sceneScaleCombo->addItems(scales);
        sceneScaleCombo->setCurrentIndex(4);
        connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(sceneScaleChanged(QString)));
        ui->Mode_ToolBar->addWidget(sceneScaleCombo);
}
//+++++++++++++++++++++++++++++Функции для редактирования текста+++++++++++++++++++++++++++++++++++
//+============================Запись настроек+====================================================
void MainWindow::readSettings()
{
    QSettings my_settings("Pete", "UML 2.0");

    my_settings.beginGroup("my_settings");

    int width_,height_,size_;
    bool bold_,underline_,italic_;
    QString font_;
    width_ = my_settings.value("width",width()).toInt();
    height_ = my_settings.value("height",height()).toInt();
    font_ = my_settings.value("font","").toString();
    size_ = my_settings.value("size_font",0).toInt();
    bold_= my_settings.value("bold",true).toBool();
    underline_ = my_settings.value("underline",true).toBool();
    italic_ = my_settings.value("italic",true).toBool();

    my_settings.endGroup();

    resize(width_,height_);

    fontSizeCombo->setCurrentIndex(size_);
    QFont font(font_,size_,-1,false);
    font.setWeight(bold_ ? QFont::Bold : QFont::Normal);
    font.setItalic(italic_);
    font.setUnderline(underline_);
    if(bold_)
        ui->Bold->setChecked(true);
    if(italic_)
        ui->Italic->setChecked(true);
    if(underline_)
        ui->Underline->setChecked(true);
    fontCombo->setCurrentFont(font);
}

void MainWindow::writeSettings()
{
    QSettings my_settings("Pete", "UML 2.0");

    my_settings.beginGroup("my_settings");
    my_settings.setValue("width",width());
    my_settings.setValue("height",height());

    my_settings.setValue("font",fontCombo->currentFont().toString());

    my_settings.setValue("size_font",fontSizeCombo->currentIndex());
    my_settings.setValue("bold",ui->Bold->isChecked());
    my_settings.setValue("italic",ui->Italic->isChecked());
    my_settings.setValue("underline",ui->Underline->isChecked());
    my_settings.endGroup();
}
//=============================Запись настроек=====================================================
//=============================Настройка сцены=====================================================
void MainWindow::sceneScaleChanged(QString scale)
{
    double newScale;
    if(scale.contains('%',Qt::CaseInsensitive))
        newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    else
        newScale = scale.toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView->matrix();
    ui->graphicsView->resetMatrix();
    ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView->scale(newScale, newScale);
}
void MainWindow::itemInserted(My_diagram_item *item)
{
  // scene->addWidget()

  /*ОТЖИмаем кнопку*/
    pointerTypeGroup->button(int(My_diagram_scene::MoveItem /*режим movement*/))->setChecked(true);//Устанавливает РЕЖИМ movement
    tab_2_Group->button(int(My_diagram_scene::MoveItem /*режим movement*/))->setChecked(true);
    State_Group->button(int(My_diagram_scene::MoveItem /*режим movement*/))->setChecked(true);
    scene->setMode(My_diagram_scene::Mode(pointerTypeGroup->checkedId()));//Устанавливаем режим треугольник, ромбик и т.д.
    buttonGroup->button(int(item->diagramType()))->setChecked(false);//Выключаем кнопку которую установили
}
void MainWindow::textInserted(QGraphicsTextItem *item)
{
    scene->setMode(My_diagram_scene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(InserttextButton)->setChecked(false);
}
void MainWindow::itemSelected(QGraphicsItem *item)
{
    My_diagram_text_item *textItem = qgraphicsitem_cast<My_diagram_text_item *>(item);
    QFont font = textItem->font();
    QColor color = textItem->defaultTextColor();
    fontCombo->setCurrentFont(font);// устанавливаем ТЕКСТ в верх чтобы было видно что он поменялся
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()/*возращаем размер шрифта*/)/*устанавливаем значение и добавляем ссылку на строку*/);
    fontSizeChanged(QString().setNum(font.pointSize()));
    ui->Bold->setChecked(font.weight() == QFont::Bold);
    ui->Italic->setChecked(font.italic());
    ui->Underline->setChecked(font.underline());
}
QWidget *MainWindow::createCellWidget(const QString &text,My_diagram_item::DiagramType type) // Делаем кнопки на форме ТООЛ кнопки с треугольником, ромбом и т.д.
{
    My_diagram_item item(type, itemMenu);

    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(button, 0, 0, Qt::AlignHCenter);

    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
void MainWindow::buttonGroupClicked(int id)// При добавлении АЙТЕМА (нового компонента для диаграммы)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons)
    {
    if (buttonGroup->button(id) != button)// Устанавливаем ЛОЖЬ тем кнопкам которые не нажаты
        button->setChecked(false);
    }
    if (id == InserttextButton) // Если мы собираемся вставить текст в диаграмму то подготавливаем сцену для ввода текста
    {
        scene->setMode(My_diagram_scene::InsertText);
    } else
    {
        scene->setItemType(My_diagram_item::DiagramType(id));//ЗАдаем тип айтема
        scene->setMode(My_diagram_scene::InsertItem); // РЕжим вставки АЙТЕМА
    }
}
void MainWindow::pointerGroupClicked(int si)// обработка режима стрелки и линии
{
    if(si == My_diagram_scene::MoveItem)
    {
        scene->setMode(My_diagram_scene::MoveItem);
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    }
    else
    {
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        scene->setMode(My_diagram_scene::InsertLine);
        switch(si)
        {
            case My_diagram_scene::Dependency:
                scene->setArrows(My_diagram_scene::Dependency);
                break;

            case My_diagram_scene::Association:
                scene->setArrows(My_diagram_scene::Association);
                break;

            case My_diagram_scene::Aggregation:
                scene->setArrows(My_diagram_scene::Aggregation);
                break;

            case My_diagram_scene::Generalization:
                scene->setArrows(My_diagram_scene::Generalization);
                break;

            case My_diagram_scene::Composition:
                scene->setArrows(My_diagram_scene::Composition);
                break;
            case My_diagram_scene::Include:
                scene->setArrows(My_diagram_scene::Include);
                break;
            case My_diagram_scene::Extend:
                scene->setArrows(My_diagram_scene::Extend);
                break;
            case My_diagram_scene::Transaction:
                scene->setArrows(My_diagram_scene::Transaction);
                break;
        }
    }
    //scene->setMode(My_diagram_scene::Mode(pointerTypeGroup->checkedId())); // В зависимости от того что мы нажали стрелку или линию тот и устанавливается режим
    //scene->setArrows();
}
//=============================Настройка сцены=====================================================
//_____________________НАстройка действий __________________________________________
void MainWindow::on_On_delete_triggered()
{
    foreach (QGraphicsItem *item, scene->selectedItems())// возвращает список выбранных АЙТЕМОВ
    {
        if (item->type() == My_diagram_arrow::Type ) // Если это стрелка то..
        {
            scene->removeItem(item);
            My_diagram_arrow *arrow = qgraphicsitem_cast<My_diagram_arrow *>(item);
            arrow->startItem()->removeArrow(arrow);// возвращает АЙтем - начало стрелки и затем удаляет его
            arrow->endItem()->removeArrow(arrow);// конец стрелки и затем удаляет его
            delete item;
            continue;

        }
        if (item->type() == My_diagram_text_item::Type && item->parentItem()!=0/* && !(scene->selectedItems().contains(item->parentItem()))*/)
        {
            item->setSelected(false);
            continue;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems())
    {
         if (item->type() == My_diagram_item::Type)
         {
             qgraphicsitem_cast<My_diagram_item *>(item)->removeArrows();
         }
         scene->removeItem(item);
         delete item;
     }
}
void MainWindow::on_On_back_triggered()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == My_diagram_item::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::on_On_front_triggered()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == My_diagram_item::Type)
        {
            zValue = item->zValue() + 0.1;
        }
    }
    selectedItem->setZValue(zValue);
}
void MainWindow::on_Bold_triggered()
{
    handleFontChange();
}

void MainWindow::on_Underline_triggered()
{
    handleFontChange();
}

void MainWindow::on_Italic_triggered()
{
    handleFontChange();
}

//_____________________НАстройка действий __________________________________________
void MainWindow::tab_2_GroupClicked(int id)
{
    if(id == My_diagram_scene::MoveItem)
        pointerGroupClicked(My_diagram_scene::MoveItem);
    if(id == My_diagram_scene::Generalization)
        pointerGroupClicked(My_diagram_scene::Generalization);
    if(id == My_diagram_scene::Dependency)
        pointerGroupClicked(My_diagram_scene::Dependency);
    if(id == My_diagram_scene::Include)
        pointerGroupClicked(My_diagram_scene::Include);
    if(id == My_diagram_scene::Extend)
        pointerGroupClicked(My_diagram_scene::Extend);
    if(id == My_diagram_scene::Association)
        pointerGroupClicked(My_diagram_scene::Association);
}
void MainWindow::State_GroupClicked(int id)
{
    if(id == My_diagram_scene::MoveItem)
        pointerGroupClicked(My_diagram_scene::MoveItem);
    if(id == My_diagram_scene::Transaction)
        pointerGroupClicked(My_diagram_scene::Transaction);
}
//Saving Scene=============================================================================-=-=-=-=-=-=-=-=-=-
void MainWindow::on_New_triggered()
{
    if (maybeSave())
    {
        scene->clear();
        setCurrentFile("");
    }
}
void MainWindow::on_Open_triggered()
{
   // QString fileName = QFileDialog::getOpenFileName(this);
    QString fileName = QFileDialog::getOpenFileName(this,"Открыть файл","","Diagram (*.dia)");
    if (!fileName.isEmpty())
        loadFile(fileName);
}
void MainWindow::on_Save_triggered()
{
    if (curFile.isEmpty())
    {
       saveAs();
    }
    else
    {
       saveFile(curFile);
    }
}

bool MainWindow::maybeSave()
{

    if (scene->items().size() != 0 && (scene->ismodified || (elms_on_scene != scene->items().size())))/*|| scene->items().size()*/
    {
        //ui->graphicsView->
        QMessageBox ret;
        QIcon Window_Title (":/Label.png");
        QPixmap Text(":/Warning.png");
        ret.setWindowIcon(Window_Title);
        ret.setIconPixmap(Text);
        ret.setText("Документ был изменен\nВы хотите сохранить изменения?");
        ret.addButton(QMessageBox::Save);
        ret.addButton(QMessageBox::Discard);
        ret.addButton(QMessageBox::Cancel);
        ret.setButtonText(QMessageBox::Save,"Сохранить");
        ret.setButtonText(QMessageBox::Discard,"Не сохранять");
        ret.setButtonText(QMessageBox::Cancel,"Отмена");
        int reft = ret.exec();
        switch (reft)
        {
        case QMessageBox::Save:
                    return save();
                    break;
        case QMessageBox::Cancel:
                    return false;
                    break;
        }
    }
    return true;
}
bool MainWindow::save()
{
    if (curFile.isEmpty())
    {
        return saveAs();
    } else
    {
        return saveFile(curFile);
    }
}
bool MainWindow::saveAs()
{
    //QString fileName = QFileDialog::getSaveFileName(this);
    QString fileName = QFileDialog::getSaveFileName(this,"Сохранить файл","","Diagram (*.dia)");
    if (fileName.isEmpty())
        return false;
    return saveFile(fileName);
}
bool MainWindow::saveFile(const QString &fileName)
{
//    QFile file(filename);
//    if (!file.open(QIODevice::WriteOnly))
//        return false;
//    QDataStream out(&file);
//    out << MagicNumber << VersionNumber;
//    out.setVersion(QDataStream::Qt_4_5);
//    writeItems(out, scene->items());
//    file.close();
//    setDirty(false);
//    return true;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly/*QFile::WriteOnly | QFile::Text*/))
    {
        QMessageBox::warning(this, tr("Мое_Приложение"),
                             tr("Ошибка при чтении файла %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QDataStream out(&file);
    writeItems(out, scene->items());
//    out << MagicNumber << VersionNumber;
    //out.setVersion(QDataStream::Qt_4_5);
    file.close();
//    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    //ui->statusBar()->showMessage(tr("Файл сохранен"), 2000);
    ui->statusBar->showMessage("Файл сохранен", 2000);
    elms_on_scene = scene->items().size();
    scene->ismodified = false;
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    //ui->textEdit->document()->setModified(false);
    MainWindow::setWindowModified(false);
    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "без_имени.ds";
    MainWindow::setWindowFilePath(shownName);
}
void MainWindow::loadFile(const QString &fileName)
{
//    QFile file(windowFilePath());
//    QDataStream in;
//    if (!openPageDesignerFile(&file, in))
//        return;
//    in.setVersion(QDataStream::Qt_4_5);
//    clear();
//    readItems(in);
//    statusBar()->showMessage(tr("Loaded %1").arg(windowFilePath()),
//                             StatusTimeout);
//    setDirty(false);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly/*QFile::ReadOnly | QFile::Text*/))
    {
        QMessageBox::warning(this, tr("Мое приложение"),
                             tr("Невозможно записать файл %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

     QDataStream in;
//     if (!openPageDesignerFile(&file, in))
//             return;
     in.setDevice(&file);
     in.setVersion(QDataStream::Qt_4_5);
     scene->clear();
     readItems(in);
//    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
//    //ui->textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    elms_on_scene = scene->items().size();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
void MainWindow::writeItems(QDataStream &out,const QList<QGraphicsItem*> &items)
{
    foreach (QGraphicsItem *item, items)
    {
        int type = static_cast<int>(item->type());
        if(type == My_diagram_item::Type)
        {
            out << type;
            out << *qgraphicsitem_cast<My_diagram_item*>(item);
        }
    }
    foreach (QGraphicsItem *item, items)
    {
        int type = static_cast<int>(item->type());
        switch (type)
        {

                   case My_diagram_arrow::Type:
                            out << type;
                            out << *qgraphicsitem_cast<My_diagram_arrow*>(item);
                            break;

                   case My_diagram_text_item::Type:
                           if((qgraphicsitem_cast<My_diagram_text_item *>(item))->ItemTextType_() == My_diagram_text_item::Margin)
                               continue;
                           out << type;
                           out << *qgraphicsitem_cast<My_diagram_text_item*>(item);
                           break;
        }
    }
}
void MainWindow::readItems(QDataStream &in)
{
    int itemType;
    while (!in.atEnd())
    {
        in >> itemType;
        switch (itemType)
        {
            case My_diagram_arrow::Type:
            {
                My_diagram_arrow *boxItem = new My_diagram_arrow();
                scene->addItem(boxItem);
                in >> boxItem;
                scene->removeItem(boxItem);
                break;
            }
            case My_diagram_item::Type:
            {
                My_diagram_item *smileyItem = new My_diagram_item(My_diagram_item::Class_,itemMenu);
                scene->addItem(smileyItem);
                in >> smileyItem;
                scene->update();
                scene->removeItem(smileyItem);
                break;
            }
            case My_diagram_text_item::Type:
            {
                My_diagram_text_item *textItem = new My_diagram_text_item();
                scene->addItem(textItem);
                in >> *textItem;       
                textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
                connect(textItem, SIGNAL(lostFocus(My_diagram_text_item*)),
                        scene, SLOT(editorLostFocus(My_diagram_text_item*)));
                connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                        scene, SIGNAL(itemSelected(QGraphicsItem*)));
//                connect(textItem,SIGNAL(items_changed()),
//                        this, SLOT(isModified()));
                textItem->setDefaultTextColor(Qt::black);
                scene->update();
                break;
            }

        }
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    } else
    {
        event->ignore();
    }
}


void MainWindow::help()
{
    QString folder = QDir::currentPath();
    QString program = "hh.exe";
    folder += "/help.chm";
    QProcess *process = new QProcess(this);
    process->start(program, QStringList() << folder);
}


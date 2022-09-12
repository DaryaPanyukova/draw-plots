#include "mainwindow.h"

#include "ui_mainwindow.h"


const double PI = 3.14159265358979323846;
const double EULER = 2.71828182845904523536;
const int INF = 1e6;
const double EPS = 1e-3;

QPen boldBlackPen(QBrush(Qt::black), 4);
QPen blackPen(QBrush(Qt::black), 1);
QPen boldRedPen(QBrush(QColor(50, 205, 50)), 5);

std::map<std::string, int> operations = {
    {"^", 1}, {"sin", 1}, {"cos", 1}, {"*", 3},  {"/", 3},  {"+", 4},
    {"-", 4}, {"_", 1},   {"log", 1}, {"ln", 1}, {"lg", 1}, {"sqrt", 1},
    {"tg", 1}, {"ctg", 1}, {"exp", 1} };

std::vector<char> signs = {'^', '*', '/', '+', '-', '(', ')', 'x', ','};
std::vector <std::string> numbers = {"x", "P" };
std::vector <std::string> prefix_functions = { "sin", "lg", "ln", "sqrt", "cos", "exp",
"tg", "ctg" };

std::vector <std::string> binary_operations = { "+", "-", "^", "/", "*" };
//std::vector <std::string> unary_operations = { "sin", "cos", "_", "log", "ln", "lg", "sqrt", "tg", "ctg", "exp" };
std::vector <std::string> unary_operations = { "sin", "cos", "_", "ln", "lg", "sqrt", "tg", "ctg", "exp" };

int Width = 1000, Height = 400;

bool ProcessBraces(std::string& element,
    std::stack<std::string>& buffer,
    std::stack<std::string>& result) {
    if (element == "(") {  // 2
        buffer.push(element);
        return false;
    } else if (element == ",") {
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            result.push(tmp);
        }
        buffer.push(",");
        return false;
    } else if (element == ")") {  // 4
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            if (tmp == ",") {
                continue;
            }
            result.push(tmp);
        }
        if (!buffer.empty() && buffer.top() == "(") {
            buffer.pop();
            return false;
        } else {
            return true;
        }
    }
}


void FirstCase(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    while (!buffer.empty() && buffer.top() != "(" &&
        operations.count(buffer.top()) != 0 &&
        operations[buffer.top()] <= operations[operation]) {
        result.push(buffer.top());
        buffer.pop();
    }
    buffer.push(operation);
}

void SecondCase(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {

    std::string x = buffer.top();

    if (operations[x] == operations[operation]) {
        if (find(prefix_functions.begin(), prefix_functions.end(), x) !=
            prefix_functions.end()) {
            FirstCase(operation, buffer, result);
        } else {
            buffer.push(operation);
        }
    } else {
        buffer.push(operation);
    }
}

void ProcessOperation(std::string& operation, std::stack<std::string>& buffer,
    std::stack<std::string>& result) {

    if (find(prefix_functions.begin(), prefix_functions.end(), operation) !=
        prefix_functions.end()) {
        buffer.push(operation);
        return;
    }

    if (operations[buffer.top()] < operations[operation]) {
        FirstCase(operation, buffer, result);
    } else
        SecondCase(operation, buffer, result);
}

inline bool IsNumber(std::string s) {
    return (s[0] >= '0' && s[0] <= '9') ||
        find(numbers.begin(), numbers.end(), s) != numbers.end();
}

bool CheckString(std::vector <std::string> &input) {
    for (int i = 1; i < input.size(); ++i) {
        /*if (find(possiible_symbols.begin(),
            possiible_symbols.end(), str[i]) == possiible_symbols.end()) {
            if (str[i] > '9' || str[i] < '0') {
                std::cout << "Unknown symbol '" << str[i];
                return false;
            }
        }*/ // заменить на нормальную проверку


        if (input[i] == "-") {
            if (find(binary_operations.begin(), binary_operations.end(), input[i - 1]) !=
                binary_operations.end() || input[i - 1] == "(" || input[i - 1] == ")")
                input[i] = '_';
        } else if (input[i - 1] == "(" || operations.count(input[i - 1]) ||
            input[i - 1] == ",") {
            if (!(input[i] == "(" || IsNumber(input[i]) ||
                find(unary_operations.begin(), unary_operations.end(), input[i]) !=
                unary_operations.end())) {
                std::cout << "ERROR";
                return false;
            }
        } else if (input[i - 1] == ")" || IsNumber(input[i])) {
            if (!(input[i] == ")" || input[i] == "," ||
                find(binary_operations.begin(), binary_operations.end(), input[i]) !=
                binary_operations.end())) {
                std::cout << "ERROR";
                return false;
            }
        }
    }
    return true;
}

std::vector <std::string> ParseString(std::string str) {
    std::vector<std::string> parsed_string;
    std::string word = "";

    parsed_string.push_back("(");
    for (int i = 0; i < str.size(); ++i) {
        if (i < str.size() - 2 && str[i] == 'e'
                && str[i+1] == 'x' && str[i+2] == 'p') {
                parsed_string.push_back("exp");
                i+=3;
        }
        if (find(signs.begin(), signs.end(), str[i]) != signs.end()) {
            if (word != "") {
                parsed_string.push_back(word);
                word = "";
            }
            parsed_string.push_back(std::string(1, str[i]));
            continue;
        }

        if (str[i] >= '0' && str[i] <= '9') {
            word += str[i];
            if (i == str.size() - 1) parsed_string.push_back(word);
            continue;
        }

        if (word != "" && IsNumber(word)) {
            parsed_string.push_back(word);
            word = "";
        }
        word += str[i];
        if (word == "pi") {
            parsed_string.push_back("P");
            word = "";
        }
        if (operations.count(word)) {
            parsed_string.push_back(word);
            word = "";
        }
    }
    parsed_string.push_back(")");
    return parsed_string;
}

double CalculateRPN(std::stack <std::string>& line, double x) {

    std::stack <double> result;
    while (!line.empty()) {
        std::string tmp = line.top();
        line.pop();
        if (operations.count(tmp)) {
            if (find(unary_operations.begin(), unary_operations.end(), tmp)
                != unary_operations.end() && tmp != "log") {
                double number = result.top();
                result.pop();
                if (tmp == "_") {
                    result.push(-number);
                } else if (tmp == "sin") {
                    result.push(sin(number));
                } else if (tmp == "cos") {
                    result.push(cos(number));
                } else if (tmp == "tg") {
                    result.push(tan(number));
                } else if (tmp == "ctg") {
                    result.push(1 / (tan(number)));
                } else if (tmp == "sqrt") {
                    result.push(sqrt(number));
                } else if (tmp == "ln") {
                    result.push(log(number));
                } else if (tmp == "lg") {
                    result.push(log10(number));
                } else if (tmp == "exp") {
                    result.push(pow(EULER,number));
                }
            } else {
                double number1 = result.top();
                result.pop();
                double number2 = result.top();
                result.pop();
                if (tmp == "+") {
                    result.push(number1 + number2);
                } else if (tmp == "-") {
                    result.push(number2 - number1);
                } else if (tmp == "*") {
                    result.push(number1 * number2);
                } else if (tmp == "/") {
                    result.push(number2 / number1);
                } else if (tmp == "^") {
                    result.push(pow(number2, number1));
                } /*else if (tmp == "log") {
                    result.push(log(number1) / log(number2));
                }*/
            }

        } else {
            double push = atoi(tmp.c_str());
            if (tmp == "P") push = PI;
            else if (tmp == "x") push = x;
            result.push(push);
        }
    }
    return result.top();
}


std::stack <std::string> ReverseStack(std::stack <std::string>& result) {
    std::cout << '\n';
    std::stack <std::string> tmp;
    while (!result.empty()) {
        tmp.push(result.top());
        result.pop();
    }
    return tmp;
}


std::stack<std::string>  GetRPN(std::string str) {
    std::vector <std::string> parsed_string = ParseString(str);
    if (!CheckString(parsed_string)) {
        std::cout << "Incorrect expression";
    }

    bool error = false;
    std::stack<std::string> result;
    std::stack<std::string> buffer;
    for (int i = 0; i < parsed_string.size(); ++i) {
        std::string element = parsed_string[i];
        if (IsNumber(element)) {
            result.push(element);
        } else {
            if (element == "(" || element == ")" || element == ",") {
                if (ProcessBraces(element, buffer, result)) {
                    error = true;
                    std::cout << "Incorrect expression";
                }
            } else if (operations.count(element) != 0 && !error) {
                ProcessOperation(element, buffer, result);
            }
        }
    }

    return result;
}


double GetValueOfFunc (std::string func, double x) {
    std::stack<std::string> rpn = GetRPN(func);
    std::stack<std::string> rpn_reversed = ReverseStack(rpn);

    double value = CalculateRPN(rpn_reversed, x);
    return value;
}


void MainWindow::ClearScene() {
    scene->clear();
    scene->setBackgroundBrush(QBrush(Qt::white));
    scene->addLine(0, -Height, 0, Height, boldBlackPen);
    scene->addLine(-Width, 0, Width, 0, boldBlackPen);


    /*for (int i = -100; i <= 100; ++i) {
     scene->addLine(-scene->mwidth,2 * i * scene -> factor, scene->mwidth,2 * i * scene -> factor, blackPen);
     scene->addLine(2 * i * scene -> factor, -scene->mheight, 2 * i * scene -> factor, scene->mheight, blackPen);
    }*/

    double delta;
    double factor = ui->graphicsView->getScaleFactor();
    if (factor  < 30) delta = 4;
    else if (factor < 50) delta = 2;
    else if (factor < 100) delta = 1;
    else delta = 0.5;

    delta *= factor;

    for (int i = -500; i <= 500; ++i) {
     scene->addLine(-Width,  i * delta, Width, i * delta, blackPen);
     scene->addLine(i * delta, -Height, i  * delta, Height, blackPen);
    }
}

void MainWindow:: DrawPlot(QString s) {
    double factor =  ui->graphicsView->getScaleFactor();
    double scale_x = factor;
    double scale_y = factor;
    double dx = 1;
    std::string func = s.toStdString();
    double y_prev = GetValueOfFunc(func, -Width/scale_x) * scale_y;

    for (double x = -Width; x < Width; x += dx) {
        double actual_x2 = (x + dx) / scale_x;
        double cur_y =  GetValueOfFunc(func, actual_x2);
        if (abs(cur_y - y_prev / scale_y) > 50) {
            y_prev = cur_y * scale_y;
            continue;
        }
        scene -> addLine(x, y_prev, x+dx, cur_y * scale_y, boldRedPen);
        y_prev = cur_y * scale_y;
    }

}

void MainWindow::Redraw() {

    ClearScene();
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
       QString s = ui->tableWidget->item(i, 0) ->text();
       DrawPlot(s);
    }

}


void MainWindow::on_lineEdit_editingFinished()
{
    QString s = ui->lineEdit->text();
    if (s == "") return;
    ui->lineEdit->clear();
    QTableWidgetItem* itm = new QTableWidgetItem(s);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount() );
    itm->setFlags(itm->flags() ^ Qt::ItemIsEditable);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, itm);

    DrawPlot(s);
}


void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    ui->tableWidget->removeRow(item->row());
    Redraw();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

    connect(ui->graphicsView, SIGNAL(signal2()), this, SLOT(Redraw()));

    scene->setSceneRect(-Width, -Height, 2*Width, 2*Height); // -width, -height, 2*width, 2*height
    ui-> graphicsView->scale(1, -1);

    ClearScene();


    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_clearButton_clicked()
{
    ui -> tableWidget -> setRowCount(0);
    Redraw();
}

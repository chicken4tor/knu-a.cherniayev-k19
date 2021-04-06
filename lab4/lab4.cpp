// 1, 5, 7, 14, 18, 21, 22

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include <map>


using namespace std;


// Генеруємо випадкові данні
random_device r;
default_random_engine reng/*(r())*/;
uniform_real_distribution<double> probability(.0, 1.);
uniform_int_distribution<int> bin_nodes(1, 100);

// 1. Структурау даних для опису дерева з довільною кількістю «дітей».
//    У вузлах дерева зберігаються цілі числа.
struct TreeNode
{
    int data{};

    vector<TreeNode *> children;

    ~TreeNode();
};

TreeNode::~TreeNode()
{
    for (auto ch : children)
    {
        delete ch;
    }
}

// 4. Додавання елемента до довільного дерева.
//    Передається значення нового елемента та вірогідність p.
//    З вірогідністю p новий елемент додається як дитина заданого вузла,
//    з вірогідністю 1-p додається за тою самою процедурою до одного з дітей
//    даного вузла (один з дітей обирається з однаковою вірогідністю).
void add_proc(TreeNode *node, int value, double p)
{
    double v = probability(reng);

    TreeNode *new_node = new TreeNode;

    new_node->data = value;

    if (v <= p || node->children.empty())
    {
        node->children.push_back(new_node);
    }
    else
    {
        int child_index = static_cast<int>(floor(v * node->children.size()));

        add_proc(node->children[child_index], value, p);
    }
}


// 5. Реалізувати функції для обчислення наступних параметрів вузла дерева:

// кількість дітей;
int children_count(TreeNode *node)
{
    return node->children.size();
}

// висота під-дерева, що починається з даного вузла;
int tree_height(TreeNode *node)
{
    int height = 1;
    vector<int> ch_height;

    ch_height.reserve(node->children.size());

    for (auto child : node->children)
    {
        ch_height.push_back(tree_height(child));
    }

    // Шукаємо найвисочійше під-дерево
    auto it = max_element(begin(ch_height), end(ch_height));

    if (it != end(ch_height))
    {
        height += *it;
    }

    return height;
}

// кількість вузлів у під-дереві, що починається з даного вузла.
int total_nodes(TreeNode *node)
{
    int childeren_count = node->children.size();

    for (auto child : node->children)
    {
        childeren_count += total_nodes(child);
    }

    return childeren_count;
}


// 7. Реалізувати функцію для виводу дерева з використанням дужок для дітей.

void print_node(TreeNode *node)
{
    cout << " " << node->data << " ";

    if (!node->children.empty())
    {
        cout << "(";
        for (auto child : node->children)
        {
            print_node(child);
        }
        cout << ")";
    }
}

void print_tree(TreeNode *node)
{
    print_node(node);

    cout << "\n";
}


// 14. Реалізувати функцію для видалення елементу з довільного дерева.
//     Передається значення елементу (якщо є кілька елементів з
//     таким значенням – видаляється будь-який один вузол).
//     Повертається видалене під-дерево.

TreeNode *delete_node(TreeNode *node, int value)
{
    TreeNode *result = nullptr;

    auto it = find_if(begin(node->children), end(node->children), [value](TreeNode *n){ return n->data == value; });

    if (it != end(node->children))
    {
        result = *it;

        node->children.erase(it);
    }
    else
    {
        for (auto ch : node->children)
        {
            result = delete_node(ch, value);

            if (result != nullptr)
            {
                break;
            }
        }
    }

    return result;
}


// 18. Реалізувати структуру даних для опису бінарного дерева.
//     Реалізувати функцію для додавання елементів відповідно
//     до значення (менші значення наліво, всі інші направо).

struct BinaryNode
{
    int data{};

    BinaryNode *left{};
    BinaryNode *right{};

    ~BinaryNode();
};

BinaryNode::~BinaryNode()
{
    delete left;
    delete right;
}

void add_node(BinaryNode *node, BinaryNode *new_node)
{
    if (new_node->data < node->data)
    {
        // add to left
        if (node->left == nullptr)
        {
            node->left = new_node;
        }
        else
        {
            add_node(node->left, new_node);
        }
    }
    else
    {
        // add to right
        if (node->right == nullptr)
        {
            node->right = new_node;
        }
        else
        {
            add_node(node->right, new_node);
        }
    }
}


// 21. Побудувати з бінарного дерева послідовне подання на основі прямого порядку.

void print_node(BinaryNode *node)
{
    cout << " " << node->data << " ";

    if (node->left != nullptr)
    {
        print_node(node->left);
    }

    if (node->right != nullptr)
    {
        print_node(node->right);
    }
}

void print_tree(BinaryNode *node)
{
    print_node(node);

    cout << "\n";
}


// 22. Реалізувати дерево для подання арифметичних виразів.
//     Підтримуються константи та змінні, а також операції +, -, *, /, піднесення до стeпеню.
//     Реалізувати спрощення виразів,
//     обчислення константних підвиразів,
//     пошук помилок (наприклад, ділення на 0).
//     Реалізувати обчислення значення виразу для заданих значень змінних.
//     Реалізувати вивід виразу у звичайній математичній нотації з мінімальною кількістю дужок.

enum TokenType
{
    NUMBER,
    PLUS,      // +
    MINUS,     // -
    MULTIPLY,  // *
    DIVIDE,    // /
    // MODULE,    // %
    POWER,     // **
    RPAREN,    // )
    LPAREN,    // (
    ASSIGN,    // =
    SEMICOLON, // ;
    ID,        // "Variable" name
    END,
};

// Тут будемо зберігати значення змінних
typedef map<string, double> environment;

struct Token
{
    TokenType type{};
    double value{};
    string name;
};

struct ExpressionNode
{
    ExpressionNode *left{};
    ExpressionNode *right{};

    Token data;

    ~ExpressionNode();
};

ExpressionNode::~ExpressionNode()
{
    delete left;
    delete right;
}

ExpressionNode *eval_const(ExpressionNode *node)
{
    if (node == nullptr)
    {
        return node;
    }

    ExpressionNode *ev_left{};
    ExpressionNode *ev_right{};

    ev_left = eval_const(node->left);
    ev_right = eval_const(node->right);

    switch (node->data.type)
    {
    case PLUS:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value + ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case MINUS:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value - ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case MULTIPLY:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value * ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case DIVIDE:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value / ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case POWER:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = pow(ev_left->data.value, ev_right->data.value);
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    default:
        node->left = ev_left;
        node->right = ev_right;
        break;
    }

    return node;
}

ExpressionNode *eval_env(ExpressionNode *node, environment &vars)
{
    if (node == nullptr)
    {
        return node;
    }

    ExpressionNode *ev_left{};
    ExpressionNode *ev_right{};

    ev_left = eval_env(node->left, vars);
    ev_right = eval_env(node->right, vars);

    if (ev_left != nullptr && ev_left->data.type == ID)
    {
        ev_left->data.type = NUMBER;
        ev_left->data.value = vars[ev_left->data.name];
    }

    if (ev_left != nullptr && ev_right->data.type == ID)
    {
        ev_right->data.type = NUMBER;
        ev_right->data.value = vars[ev_right->data.name];
    }

    switch (node->data.type)
    {
    case PLUS:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value + ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case MINUS:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value - ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case MULTIPLY:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value * ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case DIVIDE:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = ev_left->data.value / ev_right->data.value;
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    case POWER:
        if (ev_left->data.type == NUMBER && ev_right->data.type == NUMBER)
        {
            node->data.type = NUMBER;
            node->data.value = pow(ev_left->data.value, ev_right->data.value);
            node->left = node->right = nullptr;
            delete ev_left;
            delete ev_right;
        }
        break;
    default:
        node->left = ev_left;
        node->right = ev_right;
        break;
    }

    return node;
}

void eval(ExpressionNode *node, environment &vars)
{
    ExpressionNode *root = eval_const(node);

    eval_env(root, vars);
}


bool priority_is_lower(TokenType base, TokenType child)
{
    if (child == NUMBER || child == ID)
    {
        return false;
    }

    if (base == POWER && (child == MINUS || child == PLUS || child == MULTIPLY || child == DIVIDE))
    {
        return true;
    }

    if ((base == MULTIPLY || base == DIVIDE) && (child == MINUS || child == PLUS))
    {
        return true;
    }

    return false;
}

void print_expr_node(ExpressionNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    bool need_paren = false;

    if (node->left != nullptr)
    {
        need_paren = priority_is_lower(node->data.type, node->left->data.type);

        if (need_paren)
        {
            cout << "(";
        }

        print_expr_node(node->left);
    
        if (need_paren)
        {
            cout << ")";
        }
    }

    switch (node->data.type)
    {
    case MINUS:
        cout << " - ";
        break;
    case PLUS:
        cout << " + ";
        break;
    case MULTIPLY:
        cout << " * ";
        break;
    case DIVIDE:
        cout << " / ";
        break;
    case POWER:
        cout << " ** ";
        break;
    case NUMBER:
        cout << node->data.value;
        break;
    case ID:
        cout << node->data.name;
        break;
    }

    if (node->right != nullptr)
    {
        need_paren = priority_is_lower(node->data.type, node->right->data.type);

        if (need_paren)
        {
            cout << "(";
        }

        print_expr_node(node->right);

        if (need_paren)
        {
            cout << ")";
        }
    }
}

void print_expr(ExpressionNode *node)
{
    print_expr_node(node);

    cout << "\n";
}

void print_expr(ExpressionNode *node, environment &vars)
{
    bool sep = false;

    for (auto &var : vars)
    {
        if (sep)
        {
            cout << ", ";
        }

        cout << var.first << " = " << var.second;

        sep = true;
    }

    if (sep)
    {
        cout << "\n";
    }

    print_expr_node(node);

    cout << "\n";
}



int main()
{
    // Довільне дерево
    TreeNode *node = new TreeNode;

    node->data = 100;

    for (int i = 0; i < 20; ++i)
    {
        add_proc(node, i, .2);
    }

    cout << "Lab 4\n";

    cout << left << setw(13) << "Children: " << setw(5) << right << children_count(node) << "\n";
    cout << left << setw(13) << "Tree height: " << setw(5) << right << tree_height(node) << "\n";
    cout << left << setw(13) << "Nodes: " << setw(5) << right << total_nodes(node) << "\n";

    print_tree(node);

    TreeNode *deleted = delete_node(node, 8);

    cout << "Delete node with value 8:\n";

    print_tree(node);
    print_tree(deleted);

    delete deleted;
    delete node;

    // Бінарне дерево

    BinaryNode *bin_node = new BinaryNode;

    bin_node->data = 50;

    for (int i = 0; i < 20; ++i)
    {
        BinaryNode *new_node = new BinaryNode;

        new_node->data = bin_nodes(reng);

        add_node(bin_node, new_node);
    }

    print_tree(bin_node);

    delete bin_node;

    // Аріфметични вирази

    ExpressionNode *root = nullptr;
    ExpressionNode *expr = new ExpressionNode;
    ExpressionNode *arg1 = new ExpressionNode;
    ExpressionNode *arg2 = new ExpressionNode;

    root = expr;

    // 10 + 20
    expr->data.type = PLUS;
    expr->left = arg1;
    expr->right = arg2;

    arg1->data.type = NUMBER;
    arg1->data.value = 10;

    arg2->data.type = NUMBER;
    arg2->data.value = 20;

    expr = new ExpressionNode;
    arg1 = new ExpressionNode;

    expr->data.type = MULTIPLY;
    expr->left = arg1;
    expr->right = root;

    arg1->data.type = NUMBER;
    arg1->data.value = 2;

    // 2 * (10 + 20)
    root = expr;

    print_expr(root);

    eval_const(root);

    print_expr(root);

    delete root;

    // значення змінних
    environment vars;

    vars["x"] = 10;
    vars["y"] = 20;

    expr = new ExpressionNode;
    arg1 = new ExpressionNode;
    arg2 = new ExpressionNode;

    // 2 * x

    expr->data.type = MULTIPLY;
    expr->left = arg1;
    expr->right = arg2;

    arg1->data.type = NUMBER;
    arg1->data.value = 2;

    arg2->data.type = ID;
    arg2->data.name = "x";

    root = expr;

    expr = new ExpressionNode;
    arg2 = new ExpressionNode;

    // 2 * x + y
    expr->data.type = PLUS;
    expr->left = root;
    expr->right = arg2;

    arg2->data.type = ID;
    arg2->data.name = "y";

    root = expr;

    print_expr(root, vars);

    eval(root, vars);

    print_expr(root);

    delete root;
}

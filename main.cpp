#include <cctype>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <unistd.h>
#include <unordered_map>
using namespace std;

int getNumber ()
{
  int a;
  cin >> a;
  while (cin.fail())
  {
    cin.clear();
    cin.ignore();
    cin >> a;
  }
  return a;
}

void printStack(stack<string> s)
{
  while (!s.empty())
  {
    cout << s.top() << " ";
    s.pop();
  }
  cout << endl;
}

int getPriority(string op)
{
  if (op == "(")
    return 1;
  if (op == "+" || op == "-")
    return 2;
  if (op == "*" || op == "/")
    return 3;
  return 0;
}

vector<string> split(string s, string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
  {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

bool isOperator(string s)
{
  if (s == "+" || s == "-" || s == "*" || s == "/" || s == "(" || s == ")")
    return true;
  return false;
}

string infixToPostfix(string infixStr)
{
  stack<string> operators;
  string postfix = "";
  vector<string> infix = split(infixStr, " ");
  for (auto c: infix)
  {
    if (!(isOperator(c)))
    {
      postfix += c;
      postfix += " ";
      cout << c << " - операнд, отправлен в строку" << endl;
      cout << "строка: " << postfix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    }
    else if (c == "(")
    {
      operators.push(c);
      cout << c << " - открывающая скобка - помещается в стек" << endl;
      cout << "строка: " << postfix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    } 
    else if (c == ")")
    {
      while (!operators.empty() && operators.top() != "(")
      {
        postfix += operators.top();
        postfix += " ";
        operators.pop();
      }
      if (!operators.empty() && operators.top() == "(")
      {
        operators.pop();
      }
      cout << c << " - закрывающая скобка, в строку помещаются все операторы до откр. скобки" << endl;
      cout << "строка: " << postfix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    }
    else
    {
      cout << c << " - оператор, помещается в стек" << endl;
      if (!((!operators.empty() && getPriority(operators.top()) >= getPriority(c))))
      {
        operators.push(c);
        cout << "строка: " << postfix << " стэк: " ;
        printStack(operators);
        operators.pop();
        usleep(500000);
      }
      while (!operators.empty() && getPriority(operators.top()) >= getPriority(c))
      {
        postfix += operators.top();
        postfix += " ";
        cout << operators.top() << " - оператор из стека отпр. в строку, приоритет " <<
          c << " ниже или равен приоритету " << operators.top() << endl;
        operators.pop();
        cout << "строка: " << postfix << " стэк: " ;
        printStack(operators);
        usleep(500000);
      }
      operators.push(c);
    }
  }
  while (!operators.empty())
  {
    postfix += operators.top();
    postfix += " ";
    cout << operators.top() << " - оператор из стека отпр. в строку" << endl;
    operators.pop();
    cout << "строка: " << postfix << " стэк: " ;
    printStack(operators);
    usleep(500000);
  }
  return postfix;
}

string prefixToPostfix(string prefixStr)
{
  vector<string> prefix = split(prefixStr, " ");
  stack<string> st;
  for (int i = prefix.size() - 1; i >= 0; i--)
  {
    if (isOperator(prefix[i]))
    {
      string op1 = st.top();
      st.pop();
      string op2 = st.top();
      st.pop();
      string result = op1 + " " + op2 + " " + prefix[i];
      st.push(result);
    }
    else
    {
      st.push((prefix[i]));
    }
  }
  return st.top();
}

string infixToPrefix(string infixStr)
{
  stack<string> operators;
  string prefix = "";
  vector<string> infix = split(infixStr, " ");

  for (int i = infix.size() - 1; i >= 0; i--)
  {
    string c = infix[i];
    if (!(isOperator(c)))
    {
      prefix =  c + " " + prefix;
      cout << c << " - операнд, отправлен в строку" << endl;
      cout << "строка: " << prefix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    }
    else if (c == ")")
    {
      operators.push(c);
      cout << c << " - закрывающая скобка - помещается в стек" << endl;
      cout << "строка: " << prefix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    }
    else if (c == "(")
    {
      while (!operators.empty() && operators.top() != ")")
      {
        prefix = operators.top() + " " + prefix;
        operators.pop();
      }
      if (!operators.empty() && operators.top() == ")")
      {
        operators.pop();
      }
      cout << c << " - открывающая скобка, в строку помещаются все операторы до закр. скобки" << endl;
      cout << "строка: " << prefix << " стэк: " ;
      printStack(operators);
      usleep(500000);
    }
    else
    {
      cout << c << " - операция, помещается в стек" << endl;
      if (!((!operators.empty() && getPriority(operators.top()) >= getPriority(c))))
      {
        operators.push(c);
        cout << "строка: " << prefix << " стэк: " ;
        printStack(operators);
        operators.pop();
        usleep(500000);
      }
      while (!operators.empty() && getPriority(operators.top()) >= getPriority(c))
      {
        prefix = operators.top() + " " + prefix;
        cout << operators.top() << " - операция из стека отпр. в строку, приоритет " <<
          c << " ниже или равен приоритету " << operators.top() << endl;
        operators.pop();
        cout << "строка: " << prefix << " стэк: " ;
        printStack(operators);
        usleep(500000);
      }
      operators.push(c);
    }
  }
  while (!operators.empty())
  {
    prefix = operators.top() + " " + prefix;
    cout << operators.top() << " - операция из стека отпр. в строку" << endl;
    operators.pop();
    cout << "строка: " << prefix << " стэк: " ;
    printStack(operators);
    usleep(500000);
  }
  return prefix;
}

int calculateExpression(string expression)
{
  stack<int> operands;
  stack<char> operators;

  stringstream ss(expression);
  string token;
  
  while (getline(ss, token, ' '))
  {
    if (token == "+" || token == "-" || token == "*" || token == "/")
    {
      operators.push(token[0]);
      cout << token << " - оператор, заносим в стек операторов" << endl;
    }
    else
    {
      int operand = stoi(token);
      operands.push(operand);
      cout << token << " - операнд, заносим в стек операндов" << endl;
      usleep(500000);
    }
    
    while (operators.size() >= 1 && operands.size() >= 2)
    {
      int operand2 = operands.top();
      operands.pop();
      cout << "Из стэка операндов извлечен: " << operand2 << endl;
      usleep(500000);
      int operand1 = operands.top();
      operands.pop();
      cout << "Из стэка операндов извлечен: " << operand1 << endl ;
      usleep(500000);
      char op = operators.top();
      operators.pop();
      cout << "Из стэка операторов извлечен: " << op << endl;
      usleep(500000);
      
      int result;
      switch (op)
      {
        case '+':
          result = operand1 + operand2;
          break;
        case '-':
          result = operand1 - operand2;
          break;
        case '*':
          result = operand1 * operand2;
          break;
        case '/':
          result = operand1 / operand2;
          break;
      }
      operands.push(result);
      cout << operand1 << ' ' << op << ' ' << operand2 << " = " << result;
      cout << " - помещен в стек операндов";
      cout << endl;
      usleep(500000);
    }
  }  
  return operands.top();
}

bool checkCorrect(string postfix)
{
  stack<int> operands;
  stack<char> operators;

  stringstream ss(postfix);
  string token;
  
  while (getline(ss, token, ' '))
  {
    if (token == "+" || token == "-" || token == "*" || token == "/")
    {
      operators.push(token[0]);
    }
    else
    {
      int operand = stoi(token);
      operands.push(operand);
    }
    while (operators.size() >= 1 && operands.size() >= 2)
    {
      int operand2 = operands.top();
      operands.pop();
      int operand1 = operands.top();
      operands.pop();
      char op = operators.top();
      operators.pop();
      
      int result;
      switch (op)
      {
        case '+':
          result = operand1 + operand2;
          break;
        case '-':
          result = operand1 - operand2;
          break;
        case '*':
          result = operand1 * operand2;
          break;
        case '/':
          result = operand1 / operand2;
          break;
      }
      operands.push(result);
    }
  }  
  // return operands.top();
  if (operators.empty())
    return true;
  return false;
}

bool isVar(string s)
{
  for (char c : s)
  {
    if (isalpha(c))
      return true;
  }
  return false;

}

unordered_map<string, int> getVariablesValues(string infixStr)
{
  unordered_map<string, int> variableValue; 
  vector<string> infix = split(infixStr, " ");
  vector<string> vars;
  for (string c : infix)
  {
    if (isVar(c))
    {
      cout << c << " = ";
      variableValue[c] = getNumber();
    }
  }
  return variableValue;
}

bool checkAllVarsInit(string infixStr, unordered_map<string, int> variableValue) 
{
  vector<string> infix = split(infixStr, " ");
  vector<string> vars;
  for (string c : infix)
  {
    if (isVar(c))
    {
      if (variableValue.find(c) == variableValue.end())
      {
        cout << "есть неИнициализированные переменные" << endl;
        return false;
      }
    }
  }
  cout << "все переменные инициализированны" << endl;
  return true;
}

int menu()
{
	cout << "1.Преобразование выражения из инфиксной формы в постфиксную и префиксную" << endl;
	cout << "2.Проверка выражения на корректность" << endl;
	cout << "3.Вычисление выражения" << endl;
	int choice = getNumber();
	return choice;
}


int main()
{
  switch (menu())
  {
    case 1:
    {
      string infix;
      getline(cin >> ws, infix);
      unordered_map<string, int> variableValue; 
      if (!(checkAllVarsInit(infix, variableValue)))
      {
        getVariablesValues(infix);
      }
      cout << "Постфиксная запись: " << infixToPostfix(infix) << endl;
      cout << "Префиксная запись: " << infixToPrefix(infix) << endl; std::string name, title;
      break;
    }
    case 2:
    {
      cout << "1. проверка инфиксной записи" << endl;
      cout << "2. проверка постксной записи" << endl;
      cout << "3. проверка префиксной записи" << endl;
      int choiceCase2 = getNumber();
      switch (choiceCase2)
      {
        case 1:
        {
          cout << "Введите выражение в инфиксной форме: " << endl;
          string infix;
          getline(cin >> ws, infix);
          if (checkCorrect(infixToPostfix(infix)))
          {
            cout << "выражение верно" << endl;
          }
          else
          {
            cout << "выражение неверно" << endl;
          }
          break;
        }
        case 2:
        {
          cout << "Введите выражение в постфиксной форме: " << endl;
          string postfix;
          getline(cin >> ws, postfix);
          if (checkCorrect(postfix))
          {
            cout << "выражение верно" << endl;
          }
          else
          {
            cout << "выражение неверно" << endl;
          }
          break;
        }
        case 3:
        {
          cout << "Введите выражение в префиксной форме: " << endl;
          string prefix;
          getline(cin >> ws, prefix);
          if (checkCorrect(prefixToPostfix(prefix)))
          {
            cout << "выражение верно" << endl;
          }
          else
          {
            cout << "выражение неверно" << endl;
          }
          break;
        }
        default:
        {
          cout << "неверно выбран пункт меню" << endl;
        }
      }
      break;
    }
    case 3:
    {
      cout << "1. вычисление значения инфиксной записи" << endl;
      cout << "2. вычисление значнеия постксной записи" << endl;
      cout << "3. вычисление значения префиксной записи" << endl;
      int choiceCase2 = getNumber();
      switch (choiceCase2)
      {
        case 1:
        {
          cout << "Введите выражение в инфиксной форме: " << endl;
          string infix;
          getline(cin >> ws, infix);
          cout << calculateExpression(infixToPostfix(infix)) << endl;
          break;
        }
        case 2:
        {
          cout << "Введите выражение в постфиксной форме: " << endl;
          string postfix;
          getline(cin >> ws, postfix);
          cout << calculateExpression(postfix) << endl;
          break;
        }
        case 3:
        {
          cout << "Введите выражение в префиксной форме: " << endl;
          string prefix;
          getline(cin >> ws, prefix);
          cout << calculateExpression(prefixToPostfix(prefix)) << endl;
          break;
        }
        default:
        {
          cout << "неверно выбран пункт меню" << endl;
        }
      }
      break;
    }
    default:
    {
      cout << "неверно выбран пункт программы" << endl;
    }
  }
  return 0;
}

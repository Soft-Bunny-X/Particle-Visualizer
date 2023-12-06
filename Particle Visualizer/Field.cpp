#include "Field.h"

Field::Field() :Field("0", "0", "0", glm::vec3(0.0, 0.0, 0.0))
{
}

Field::Field(std::string defText_x, std::string defText_y, std::string defText_z, glm::vec3 initPos):
	fieldDef_x(defText_x), fieldDef_y(defText_y), fieldDef_z(defText_z), optBuffer_x(), optBuffer_y(), optBuffer_z(), topNode_x(nullptr), topNode_y(nullptr), topNode_z(nullptr), testPos(initPos)
{
	/*extractOpElement(fieldDef_x, optBuffer_x);
	createOpTree(optBuffer_x.begin(), optBuffer_x.end(), &topNode_x);
	buildTree_power(&topNode_x);
	buildTree_multi_div(&topNode_x);
	extractOpElement(fieldDef_y, optBuffer_y);
	createOpTree(optBuffer_y.begin(), optBuffer_y.end(), &topNode_y);
	buildTree_power(&topNode_y);
	buildTree_multi_div(&topNode_y);
	extractOpElement(fieldDef_z, optBuffer_z);
	createOpTree(optBuffer_z.begin(), optBuffer_z.end(), &topNode_z);
	buildTree_power(&topNode_z);
	buildTree_multi_div(&topNode_z);*/
}


void Field::setupField(std::string fieldName, bool fromFile, std::string fileName)
{
	if (fromFile) {
		std::fstream in_file{fileName, std::ios::in};
		if (in_file.is_open()) {
			std::string description{""};
			std::getline(in_file, description);
			std::getline(in_file, fieldDef_x);
			std::getline(in_file, description);
			std::getline(in_file, fieldDef_y);
			std::getline(in_file, description);
			std::getline(in_file, fieldDef_z);
		}
		in_file.close();
	}
	else {
		std::cout << "\n\n";
		std::cout << "========================================\n\n";
		std::cout << "Instruction on Entering The Field Configuration:\n\n";
		std::cout << "* Multiplication must be explicitly expressed. For examply, product of two variables written as \'xy\' is invalid. Instead, write it as \'x*y\'.\n\n";
		std::cout << "* Only three functions are supported in the current version: cos(), sin(), and ln(). Inverse trig functions are not yet supported.\n\n";
		std::cout << "* When expressing square root, express it in the power form. For example, \'x^(1/2)\'.\n\n";
		std::cout << "* Common constants in math and science are not supported. Express them explicitly as numbers with desired accuracy. For example, instead of writting \'pi\', you should write \'3.1415926\'.\n\n";
		std::cout << "* Only 3 variables are available: x, y, and z. They represent the components of the location in Cartesian coordinate. When expressing them, both lowercase and uppercase are valid.\n\n";
		std::cout << "* Make sure to enclose function argument with parenthesis. For example, \'cosx\' would be invalid. Instead, write \'cos(x)\'.\n\n";
		std::cout << "* When consecutive exponentials appear, the order of operation is from left to right. For example, \'2^3^4 = (2^3)^4\'.\n\n";
		std::cout << "* Complex numbers not supported.\n\n";
		std::cout << "========================================\n\n";

		while (true) {
			fieldDef_x.assign("");
			fieldDef_y.assign("");
			fieldDef_z.assign("");

			std::cout << "\n----------------------------------------\n";
			std::cout << "Enter the expression for the x-component of the " << fieldName << " field:\n\n";
			std::cin >> fieldDef_x;
			std::cout << "\n----------------------------------------\n";
			std::cout << "Enter the expression for the y-component of the " << fieldName << " field:\n\n";
			std::cin >> fieldDef_y;
			std::cout << "\n----------------------------------------\n";
			std::cout << "Enter the expression for the z-component of the " << fieldName << " field:\n\n";
			std::cin >> fieldDef_z;

			std::string option;

			std::cout << "\n----------------------------------------\n";
			std::cout << "Confirm the expressions (enter \'Y\' to proceed to the next step, enter \'N\' to re-enter the expressions):\n\n";
			std::cin >> option;

			if (option == "y" || option == "Y") {
				break;
			}
		}
	}

	extractOpElement(fieldDef_x, optBuffer_x);
	createOpTree(optBuffer_x.begin(), optBuffer_x.end(), &topNode_x);
	buildTree_power(&topNode_x);
	buildTree_multi_div(&topNode_x);
	extractOpElement(fieldDef_y, optBuffer_y);
	createOpTree(optBuffer_y.begin(), optBuffer_y.end(), &topNode_y);
	buildTree_power(&topNode_y);
	buildTree_multi_div(&topNode_y);
	extractOpElement(fieldDef_z, optBuffer_z);
	createOpTree(optBuffer_z.begin(), optBuffer_z.end(), &topNode_z);
	buildTree_power(&topNode_z);
	buildTree_multi_div(&topNode_z);
}

bool Field::isDigit(char testChar)
{
	if (testChar == '.' || (testChar >= '0' && testChar <= '9')) {
		return true;
	}
	else {
		return false;
	}
}

bool Field::isBinaryOperator(ElementType elementType)
{
	//Refer back to the definition of ElementType
	if (elementType >= OP_OPERATOR_ADDITION && elementType <= OP_OPERATOR_POWER) {
		return true;
	}
	else {
		return false;
	}
}

bool Field::isOperant(ElementType elementType)
{
	if (elementType == OP_VARIABLE_X || elementType == OP_VARIABLE_Y || elementType == OP_VARIABLE_Z || elementType == OP_VARIABLE_T || elementType == OP_NUMBER) {
		return true;
	}
	else {
		return false;
	}
}

double Field::elementToNumber(OperationElement element)
{
	if (element.eleType == OP_NUMBER) {
		return element.number;
	}
	else if (element.eleType == OP_VARIABLE_X) {
		return testPos.x;
	}
	else if (element.eleType == OP_VARIABLE_Y) {
		return testPos.y;
	}
	else if (element.eleType == OP_VARIABLE_Z) {
		return testPos.z;
	}
	else if (element.eleType == OP_VARIABLE_T) {
		return currentTime;
	}
}



ElementType Field::typeOfElement(std::string &element)
{
	if (element == "x" || element == "X") {
		return OP_VARIABLE_X;
	}
	else if (element == "y" || element == "Y") {
		return OP_VARIABLE_Y;
	}
	else if (element == "z" || element == "Z") {
		return OP_VARIABLE_Z;
	}
	else if (element == "t" || element == "T") {
		return OP_VARIABLE_T;
	}
	else if (element == "+") {
		return OP_OPERATOR_ADDITION;
	}
	else if (element == "-") {
		return OP_OPERATOR_SUBTRACTION;
	}
	else if (element == "*") {
		return OP_OPERATOR_MULTIPLICATION;
	}
	else if (element == "/") {
		return OP_OPERATOR_DIVISION;
	}
	else if (element == "^") {
		return OP_OPERATOR_POWER;
	}
	else if (element == "cos") {
		return OP_FUNCTION_COS;
	}
	else if (element == "sin") {
		return OP_FUNCTION_SIN;
	}
	else if (element == "ln") {
		return OP_FUNCTION_LN;
	}
	else if (element[0] == '.' || (element[0] >= '0' && element[0] <= '9')) {
		return OP_NUMBER;
	}
	else if (element == "(") {
		return OP_PARENTHESIS_LEFT;
	}
	else if (element == ")") {
		return OP_PARENTHESIS_RIGHT;
	}
	else {
		return OP_NULL;
	}
}

void Field::extractOpElement(std::string& expression, std::vector<OperationElement>& optBuffer)
{
	std::string numTemp{""};
	std::string opTemp{""};

	for (auto i = expression.begin(); i != expression.end(); i++) {
		if (*i == ' ') {
			continue;
		}
		else if (isDigit(*i)) {
			numTemp.push_back(*i);
			if ((i + 1) == expression.end() || !isDigit(*(i + 1))) {
				optBuffer.push_back(OperationElement(OP_NUMBER, std::stof(numTemp)));
				numTemp.assign("");
			}
		}
		else if (*i == 'c' && *(i + 1) == 'o' && *(i + 2) == 's') {
			optBuffer.push_back(OperationElement(OP_FUNCTION_COS, 0.0));
			i += 2;
		}
		else if (*i == 's' && *(i + 1) == 'i' && *(i + 2) == 'n') {
			optBuffer.push_back(OperationElement(OP_FUNCTION_SIN, 0.0));
			i += 2;
		}
		else if (*i == 'l' && *(i + 1) == 'n') {
			optBuffer.push_back(OperationElement(OP_FUNCTION_LN, 0.0));
			i++;
		}
		else {
			opTemp.push_back(*i);
			optBuffer.push_back(OperationElement(typeOfElement(opTemp), 0.0));
			opTemp.assign("");
		}
	}
}

void Field::createOpTree(std::vector<OperationElement>::iterator begin, std::vector<OperationElement>::iterator end, treeElement** topNode)
{
	size_t parenthesisCount{ 0 };
	std::vector<OperationElement>::iterator closingParenthesis{ end };
	*topNode = new treeElement();

	if ((*begin).eleType == OP_FUNCTION_COS || (*begin).eleType == OP_FUNCTION_SIN || (*begin).eleType == OP_FUNCTION_LN) {
		(*topNode)->eleType = OP_COMPOSITE;
		(*topNode)->opType = (*begin).eleType;
		parenthesisCount = 0;
		for (auto i = begin + 1; i != end; i++) {
			if ((*i).eleType == OP_PARENTHESIS_LEFT) {
				parenthesisCount++;
			}
			else if ((*i).eleType == OP_PARENTHESIS_RIGHT) {
				parenthesisCount--;
			}

			if (parenthesisCount == 0) {
				closingParenthesis = i;
				break;
			}
		}
		if (closingParenthesis + 1 == end) {
			createOpTree(begin + 2, closingParenthesis, &((*topNode)->subPtr));
		}
		else {
			(*topNode)->opType = (*(closingParenthesis + 1)).eleType;
			createOpTree(closingParenthesis + 2, end, &((*topNode)->nextPtr));
			createOpTree(begin + 2, closingParenthesis, &((*topNode)->subPtr));
		}
	}
	else if ((*begin).eleType == OP_PARENTHESIS_LEFT) {
		(*topNode)->eleType = OP_COMPOSITE;
		parenthesisCount = 0;
		for (auto i = begin; i != end; i++) {
			if ((*i).eleType == OP_PARENTHESIS_LEFT) {
				parenthesisCount++;
			}
			else if ((*i).eleType == OP_PARENTHESIS_RIGHT) {
				parenthesisCount--;
			}

			if (parenthesisCount == 0) {
				closingParenthesis = i;
				break;
			}
		}
		if (closingParenthesis + 1 == end) {
			createOpTree(begin + 1, closingParenthesis, &((*topNode)->subPtr));
		}
		else {
			(*topNode)->opType = (*(closingParenthesis + 1)).eleType;
			createOpTree(closingParenthesis + 2, end, &((*topNode)->nextPtr));
			createOpTree(begin + 1, closingParenthesis, &((*topNode)->subPtr));
		}
	}
	else if ((*begin).eleType == OP_OPERATOR_SUBTRACTION) {
		(*topNode)->eleType = OP_NUMBER;
		(*topNode)->opType = OP_OPERATOR_SUBTRACTION;
		(*topNode)->value = 0.0;
		createOpTree(begin + 1, end, &((*topNode)->nextPtr));
	}
	else if (isOperant((*begin).eleType)) {
		(*topNode)->eleType = (*begin).eleType;

		if ((*begin).eleType == OP_NUMBER) {
			(*topNode)->value = (*begin).number;
		}

		if (begin + 1 != end) {
			(*topNode)->opType = (*(begin + 1)).eleType;
			createOpTree(begin + 2, end, &((*topNode)->nextPtr));
		}
	}
	else {
		throw std::runtime_error("Invalid expression for the field component");
	}
}

void Field::buildTree_power(treeElement** topNode)
{
	if (*topNode != nullptr) {
		if ((*topNode)->nextPtr != nullptr) {
			if (((*topNode)->opType == OP_OPERATOR_POWER) && ((*topNode)->nextPtr)->nextPtr != nullptr) {
				treeElement* temp = *topNode;
				*topNode = new treeElement(OP_COMPOSITE, (temp->nextPtr)->opType, 0.0, temp, (temp->nextPtr)->nextPtr);
				(temp->nextPtr)->nextPtr = nullptr;
			}
			buildTree_power(&((*topNode)->nextPtr));
		}
		if ((*topNode)->subPtr != nullptr) {
			buildTree_power(&((*topNode)->subPtr));
		}
	}
}

void Field::buildTree_multi_div(treeElement** topNode)
{
	if (*topNode != nullptr) {
		if ((*topNode)->nextPtr != nullptr) {
			if (((*topNode)->opType == OP_OPERATOR_MULTIPLICATION || (*topNode)->opType == OP_OPERATOR_DIVISION) && ((*topNode)->nextPtr)->nextPtr != nullptr) {
				treeElement* temp = *topNode;
				*topNode = new treeElement(OP_COMPOSITE, (temp->nextPtr)->opType, 0.0, temp, (temp->nextPtr)->nextPtr);
				(temp->nextPtr)->nextPtr = nullptr;
			}
			buildTree_multi_div(&((*topNode)->nextPtr));
		}
		if ((*topNode)->subPtr != nullptr) {
			buildTree_multi_div(&((*topNode)->subPtr));
		}
	}
}

void Field::destroyOpTree(treeElement* topNode)
{
	if (topNode->nextPtr != nullptr) {
		destroyOpTree(topNode->nextPtr);
	}

	if (topNode->subPtr != nullptr) {
		destroyOpTree(topNode->subPtr);
	}

	if (topNode->subPtr == nullptr && topNode->nextPtr == nullptr) {
		delete topNode;
	}
}

double Field::evaluate(treeElement* topNode)
{
	double operant1{0.0};

	if (topNode->eleType == OP_COMPOSITE) {
		switch (topNode->opType) {
		case OP_NULL:
			return evaluate(topNode->subPtr);
		case OP_FUNCTION_COS:
			return (double)glm::cos(evaluate(topNode->subPtr));
		case OP_FUNCTION_SIN:
			return (double)glm::sin(evaluate(topNode->subPtr));
		case OP_FUNCTION_LN:
			return (double)glm::log(evaluate(topNode->subPtr));
		default:
			break;
		}
	}
	else if (topNode->subPtr == nullptr) {
		switch (topNode->eleType) {
		case OP_NUMBER:
			operant1 = topNode->value;
			break;
		case OP_VARIABLE_X:
			operant1 = testPos.x;
			break;
		case OP_VARIABLE_Y:
			operant1 = testPos.y;
			break;
		case OP_VARIABLE_Z:
			operant1 = testPos.z;
			break;
		case OP_VARIABLE_T:
			operant1 = currentTime;
			break;
		default:
			break;
		}

		if (topNode->nextPtr == nullptr) {
			return operant1;
		}
		else {
			switch (topNode->opType) {
			case OP_OPERATOR_ADDITION:
				return (operant1 + evaluate(topNode->nextPtr));
			case OP_OPERATOR_SUBTRACTION:
				return (operant1 - evaluate(topNode->nextPtr));
			case OP_OPERATOR_MULTIPLICATION:
				return (operant1 * evaluate(topNode->nextPtr));
			case OP_OPERATOR_DIVISION:
				if (evaluate(topNode->nextPtr) == 0.0) {
					return (operant1 / (std::numeric_limits<double>::min)());//in case when the denominator is zero
				}
				else {
					return (operant1 / evaluate(topNode->nextPtr));
				}
			case OP_OPERATOR_POWER:
				return (double)glm::pow(operant1, evaluate(topNode->nextPtr));
			default:
				break;
			}
		}
	}
	else {
		if (topNode->nextPtr == nullptr) {
			return evaluate(topNode->subPtr);
		}
		else {
			switch (topNode->opType) {
			case OP_OPERATOR_ADDITION:
				return (evaluate(topNode->subPtr) + evaluate(topNode->nextPtr));
			case OP_OPERATOR_SUBTRACTION:
				return (evaluate(topNode->subPtr) - evaluate(topNode->nextPtr));
			case OP_OPERATOR_MULTIPLICATION:
				return (evaluate(topNode->subPtr) * evaluate(topNode->nextPtr));
			case OP_OPERATOR_DIVISION:
				if (evaluate(topNode->nextPtr) == 0.0) {
					return (evaluate(topNode->subPtr) / (std::numeric_limits<double>::min)());//in case when the denominator is zero
				}
				else {
					return (evaluate(topNode->subPtr) / evaluate(topNode->nextPtr));
				}
			case OP_OPERATOR_POWER:
				return (double)glm::pow(evaluate(topNode->subPtr), evaluate(topNode->nextPtr));
			default:
				break;
			}
		}
	}
}

glm::vec3 Field::getFieldVector(double currentTime, glm::vec3 pos)
{
	this->currentTime = currentTime;
	testPos = pos;
	return glm::vec3(evaluate(topNode_x), evaluate(topNode_y), evaluate(topNode_z));
}

Field::~Field()
{
	destroyOpTree(topNode_x);
	destroyOpTree(topNode_y);
	destroyOpTree(topNode_z);
}
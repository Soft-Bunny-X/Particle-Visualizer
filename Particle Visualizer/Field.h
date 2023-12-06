#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <limits>
#include <fstream>

enum ElementType {
	OP_NULL,
	OP_VARIABLE_X,
	OP_VARIABLE_Y,
	OP_VARIABLE_Z,
	OP_VARIABLE_T,
	OP_OPERATOR_ADDITION,
	OP_OPERATOR_SUBTRACTION,
	OP_OPERATOR_MULTIPLICATION,
	OP_OPERATOR_DIVISION,
	OP_OPERATOR_POWER,
	OP_FUNCTION_COS,
	OP_FUNCTION_SIN,
	OP_FUNCTION_LN,
	OP_NUMBER,
	OP_PARENTHESIS_LEFT,
	OP_PARENTHESIS_RIGHT,
	OP_COMPOSITE
};

struct OperationElement {
	ElementType eleType;
	float number;

	OperationElement() {
		this->eleType = OP_NULL;
		this->number = 0.0f;
	}

	OperationElement(ElementType opType, float number) {
		this->eleType = opType;
		this->number = number;
	}
};

struct treeElement {
	ElementType eleType;
	ElementType opType;
	float value;
	treeElement* subPtr;
	treeElement* nextPtr;

	treeElement() {
		this->eleType = OP_NULL;
		this->opType = OP_NULL;
		this->value = 0.0;
		this->subPtr = nullptr;
		this->nextPtr = nullptr;
	}

	treeElement(ElementType eleType, ElementType opType, float value, treeElement* subPtr, treeElement* nextPtr) {
		this->eleType = eleType;
		this->opType = opType;
		this->value = value;
		this->subPtr = subPtr;
		this->nextPtr = nextPtr;
	}
};

class Field
{
public:
	Field();

	Field(std::string defText_x, std::string defText_y, std::string defText_z, glm::vec3 initPos);

	void setupField(std::string fieldName, bool fromFile, std::string fileName);

	

	glm::vec3 getFieldVector(double currentTime, glm::vec3 pos);

	~Field();
private:
	std::string fieldDef_x;
	std::string fieldDef_y;
	std::string fieldDef_z;
	std::vector<OperationElement> optBuffer_x;
	std::vector<OperationElement> optBuffer_y;
	std::vector<OperationElement> optBuffer_z;
	treeElement* topNode_x;
	treeElement* topNode_y;
	treeElement* topNode_z;

	glm::vec3 testPos;
	double currentTime;

	ElementType typeOfElement(std::string &element);
	
	void extractOpElement(std::string &expression, std::vector<OperationElement> &optBuffer);

	void createOpTree(std::vector<OperationElement>::iterator begin, std::vector<OperationElement>::iterator end, treeElement** topNode);
	void buildTree_power(treeElement** topNode);
	void buildTree_multi_div(treeElement** topNode);
	void destroyOpTree(treeElement* topNode);

	bool isDigit(char testChar);
	bool isBinaryOperator(ElementType elementType);
	bool isOperant(ElementType elementType);

	double elementToNumber(OperationElement element);

	double evaluate(treeElement* topNode);
};


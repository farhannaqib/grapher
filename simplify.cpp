#include <math.h>
#include <string>
#include <iostream>

#include "tokenizer.h"
#include "ast.h"
#include "simplify.h"

// Helper method to determine whether 
// a branch is a number node
bool isValidNum(ASTNode* node) {
    if (node->leftChild || node->rightChild) return false;
    if (node->token.type == TokenType::NUM) return true;
    return false;
}

void simplify(ASTNode* tree) {
    if (!tree) return;
    // functions w two args get parsed w a comma to maintain
    // the order of args, this removes the comma for simplifying
    if (tree->rightChild && tree->rightChild->token.type == TokenType::COMMA) {
        ASTNode* left = tree->rightChild->leftChild;
        ASTNode* right = tree->rightChild->rightChild;
        tree->rightChild->leftChild = nullptr;
        tree->rightChild->rightChild = nullptr;
        delete tree->rightChild;
        tree->leftChild = left;
        tree->rightChild = right;
    }
    simplify(tree->leftChild);
    simplify(tree->rightChild);

    // considers how many nodes that we can use for calculations
    // are available
    int numOfSingleChildren = 0;
    if (tree->leftChild && isValidNum(tree->leftChild)) 
    numOfSingleChildren++;
    if (tree->rightChild && isValidNum(tree->rightChild)) 
    numOfSingleChildren++;

    if (numOfSingleChildren == 1) {
        double child = std::stod(tree->rightChild->token.data);
        delete tree->rightChild;
        tree->rightChild = nullptr;
        std::string newData = "";

        switch(tree->token.type) {
            case (TokenType::LN):
                newData = std::to_string(log(child));
                break;
            case (TokenType::SIN):
                newData = std::to_string(sin(child));
                break;
            case (TokenType::COS):
                newData = std::to_string(cos(child));
                break;
            case (TokenType::TAN):
                newData = std::to_string(tan(child));
                break;
            case (TokenType::CSC):
                newData = std::to_string(1.0/sin(child));
                break;
            case (TokenType::SEC):
                newData = std::to_string(1.0/cos(child));
                break;
            case (TokenType::COT):
                newData = std::to_string(1.0/tan(child));
                break;
            case (TokenType::SQRT):
                newData = std::to_string(sqrt(child));
                break;
            case (TokenType::ABS):
                newData = std::to_string(abs(child));
                break;
            case (TokenType::FLOOR):
                newData = std::to_string(floor(child));
                break;
            case (TokenType::CIEL):
                newData = std::to_string(ceil(child));
                break;
            default:
                newData = "";
                break;
        }
        tree->token = NumToken(newData);
    }

    if (numOfSingleChildren == 2) { // token has to be binary
        double left = std::stod(tree->leftChild->token.data);
        double right = std::stod(tree->rightChild->token.data);
        delete tree->leftChild;
        tree->leftChild = nullptr;
        delete tree->rightChild;
        tree->rightChild = nullptr;
        std::string newData = "";

        switch(tree->token.type) {
            case (TokenType::ADD):
                newData = std::to_string(left+right);
                break;
            case (TokenType::SUB):
                newData = std::to_string(left-right);
                break;
            case (TokenType::MUL):
                newData = std::to_string(left*right);
                break;
            case (TokenType::DIV):
                newData = std::to_string(left/right);
                break;
            case (TokenType::POW):
                newData = std::to_string(pow(left,right));
                break;
            case (TokenType::MOD):
                newData = std::to_string((int) left % (int) right);
                break;
            case (TokenType::MAX):
                newData = std::to_string(std::max(left, right));
                break;
            case (TokenType::MIN):
                newData = std::to_string(std::min(left, right));
                break;
            case(TokenType::LOG):
                newData = std::to_string(log(left)/log(right));
                break;
            default:
                newData = "";
                break;
        }
        tree->token = NumToken(newData);
    }
}

int main() {
    std::string inputs[] {
        "1+2", "1+2*3", "1+2*3-4",
        "1^2", "0+1^2", "E+1", "1+s",
        "1--2", "1--(-2)", "1+-2", "1-+2",
        "-1", "SIN(PI)", "MAX(2, 3)", "MAX(1+4,3)", 
        "LOG(2,2)", "LOG(4,2)",
        "(4+5)", "0-(1)", "0-(4+5)", "-(4+5)"
    };
    
    //for (std::string input : inputs){
    //    ASTNode* output = stringtoAST(input);
    //    std::cout << ASTtoString(output) << std::endl;
    //}

    std::string inputs2[] {
        "1+2", "1+2*3", "1+2*3-4",
        "1^2", "0+1^2", "E+1"
    };

    for (std::string input : inputs) {
        ASTNode* output = stringtoAST(input);
        simplify(output);
        std::cout << ASTtoString(output) << std::endl;
    }

    return 0;
}
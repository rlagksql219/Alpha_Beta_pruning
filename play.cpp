#include <iostream>
#include <stdlib.h>
#include <vector>
#include <float.h>
#include <algorithm>

using namespace std;

int maxdepth = 0;

typedef struct TreeNode { /* 트리 노드 구조체 */
	int stone; /* 돌 번호 */
	int depth; /* Tree의 depth */
	double evaluation; /* 평가한 evaluation 값 */
	int childNum; /* 자식노드 개수 */
	vector<TreeNode*> childs; /* 자식 노드 */
} Node;


Node* makeTree(vector<int> input); /* 가져간 돌의 리스트만큼 트리 생성 */
void InitNode(Node* node, int depth, int stoneNum); /* 노드 초기화 */
int getNumOfPossibleCase(vector<int>& caseList, vector<int> input, int stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

void Max_bestMove(Node* root, vector<int> input, int stoneNum); /* Best Move 결정 */
void Min_bestMove(Node* root, vector<int> input, int stoneNum); /* Best Move 결정 */
double Max(Node* root, vector<int> input, int stoneNum, double alpha, double beta); /* Evaluation 값 중 Max 값을 구함 */
double Min(Node* root, vector<int> input, int stoneNum, double alpha, double beta); /* Evaluation 값 중 Min 값을 구함 */
double evaluate_Max(vector<int> input, int stoneNum); /* 현재 돌의 Evaluation 계산 */
double evaluate_Min(vector<int> input, int stoneNum); /* 현재 돌의 Evaluation 계산 */

int count_decimal(int stoneNum); /* 소수의 개수 계산 */
void PrintOutput(int bestMove, Node* root, vector<int> input); /* Output 출력 */


int main(int argc, char* argv[]) {
	int stoneNum, inputNum; /* 전체 돌 개수, 가져간 돌 개수 */
	vector<int> input; /* 가져간 돌 리스트 */
	Node* root;

	/* 인자가 제대로 주어지지 않은 경우 */
	if (argc < 3) {
		printf("입력 형태가 올바르지 않습니다.\n");
		exit(1);
	}

	stoneNum = atoi(argv[1]);
	inputNum = atoi(argv[2]);

	/* input 초기화 */
	for (int i = 3; i < argc; i++) {
		input.push_back(atoi(argv[i]));
	}

	root = makeTree(input); /* 가져간 돌의 리스트만큼 트리 생성 */

	if (inputNum > 0) {
		if (inputNum % 2 != 0) {
			Max_bestMove(root, input, stoneNum);
		}
		else {
			Min_bestMove(root, input, stoneNum);
		}
	}
	else {
		Min_bestMove(root, input, stoneNum);
	}

	return 0;
}

/**
함 수 : makeTree(vector<int> input)
기 능 : 가져간 돌의 리스트만큼 트리 생성헤서 마지막으로 만든 노드 리턴
*/
Node* makeTree(vector<int> input) {
	Node* currNode = new Node;
	Node* parentNode = NULL;

	for (unsigned int i = 0; i < input.size(); i++) {
		currNode->stone = input[i];
		currNode->depth = 0;
		currNode->evaluation = 0;
		currNode->childNum = 0;

		parentNode = currNode;
		if (i != input.size() - 1) {
			currNode->childs.push_back(new Node);
			currNode = currNode->childs.front();
		}

	}

	if (input.size() > 0)
		return parentNode;
	else {
		InitNode(currNode, 0, 0);
		return currNode;
	}
}

/**
함 수 : InitNode(Node* node, int depth, int stoneNum)
기 능 : 노드의 depth, evaluation, childNum 정보 초기화
*/
void InitNode(Node* node, int depth, int stoneNum) {
	node->depth = depth;
	node->evaluation = 0;
	node->childNum = 0;
}

/**
함 수 : getNumOfPossibleCase(vector<int>& caseList, vector<int> input, int stoneNum)
기 능 : 다음 차례에 가져갈 수 있는 돌의 리스트를 생성하고, 그 갯수 리턴
*/
int getNumOfPossibleCase(vector<int>& caseList, vector<int> input, int stoneNum) {
	int case_num = 0;
	double index = (double)(stoneNum / 2);

	/* 가져간 돌이 없는 경우 */
	if (input.empty()) {
		for (int i = 1; i <= index; i++) {
			if ((i % 2) != 0) {
				caseList.push_back(i);
				case_num++;
			}
		}
		return case_num;
	}

	else {
		int Last = input.back();

		/* 마지막으로 가져간 돌의 배수 또는 인수 계산 */
		for (int i = 1; i <= stoneNum; i++) {
			if (i % Last == 0 || Last % i == 0) {
				bool isExistInInput = false;

				/* 전에 골랐던 돌인지 판단 */
				for (unsigned int j = 0; j < input.size(); j++) {
					if (input[j] == i) {
						isExistInInput = true;
						break;
					}
				}

				if (!isExistInInput) {
					caseList.push_back(i);
					case_num++;
				}
			}
		}
		return case_num;
	}
}

/**
함 수 : Max_bestMove(Node* root, vector<int> input, int stoneNum)
기 능 : Best Move를 Alpha-Beta pruning 알고리즘을 통해 구함
*/
void Max_bestMove(Node* root, vector<int> input, int stoneNum) {
	vector<int> caseList; /* 선택할 수 있는 돌 리스트 */
	double alpha = FLT_MIN;
	double beta = FLT_MAX;

	int case_num, bestMove = 0;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 가능한 수만큼 alpha-beta 알고리즘을 사용하여 수를 계산 */
	for (int i = 0; i < case_num; i++) {
		root->childs.push_back(new Node); /* 새로운 노드 생성 */
		InitNode(root->childs[i], (root->depth) + 1, stoneNum); /* 노드 초기화 */

		/* 선택할 수 있는 돌 중에 하나를 가져감 */
		input.push_back(caseList[i]);
		root->childs[i]->stone = caseList[i];

		double newEval = Min(root->childs[i], input, stoneNum, alpha, beta); /* Min 함수호출로 값을 구해서 */
		root->childs[i]->evaluation = newEval; /* 노드에 저장 */

		input.pop_back(); /* 선택할 수 있는 돌 중에 가져간 돌을 다시 내려놓음 */

		if (newEval > alpha) {
			alpha = newEval;
			bestMove = caseList[i];
			root->evaluation = alpha;
		}
	}

	root->childNum = case_num;

	PrintOutput(bestMove, root, input); /* Output 출력 */
}

/**
함 수 : Min_bestMove(Node* root, vector<int> input, int stoneNum)
기 능 : Best Move를 Alpha-Beta pruning 알고리즘을 통해 구함
*/
void Min_bestMove(Node* root, vector<int> input, int stoneNum) {
	vector<int> caseList; /* 선택할 수 있는 돌 리스트 */
	double alpha = FLT_MIN;
	double beta = FLT_MAX;

	int case_num, bestMove = 0;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 가능한 수만큼 alpha-beta 알고리즘을 사용하여 수를 계산 */
	for (int i = 0; i < case_num; i++) {
		root->childs.push_back(new Node); /* 새로운 노드 생성 */
		InitNode(root->childs[i], (root->depth) + 1, stoneNum); /* 노드 초기화 */

		/* 선택할 수 있는 돌 중에 하나를 가져감 */
		input.push_back(caseList[i]);
		root->childs[i]->stone = caseList[i];

		double newEval = Max(root->childs[i], input, stoneNum, alpha, beta); /* Max 함수호출로 값을 구해서 */
		root->childs[i]->evaluation = newEval; /* 노드에 저장 */

		input.pop_back(); /* 선택할 수 있는 돌 중에 가져간 돌을 다시 내려놓음 */

		if (newEval < beta) {
			beta = newEval;
			bestMove = caseList[i];
			root->evaluation = beta;
		}
	}

	root->childNum = case_num;

	PrintOutput(bestMove, root, input); /* Output 출력 */
}

/**
함 수 : Max(Node* root, vector<int> input, int stoneNum, double alpha, double beta)
기 능 : 현재 자식 노드들의 Evaluation 값 중 제일 큰 값으로 저장하는 함수
*/
double Max(Node* root, vector<int> input, int stoneNum, double alpha, double beta) {
	vector<int> caseList;
	int case_num = 0;

	if (root->depth > maxdepth)
		maxdepth = root->depth;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 다음 차례에 가져갈 수 있는 돌이 없는 경우 */
	if (case_num == 0) {
		return evaluate_Max(input, stoneNum); /* 현재 돌의 Evaluation 계산 */
	}

	/* 가능한 수만큼 alpha-beta 알고리즘을 사용하여 수를 계산 */
	for (int i = 0; i < case_num; i++) {

		/* alpha-cut */
		if (alpha >= beta)
			continue;

		root->childs.push_back(new Node); /* 새로운 노드 생성 */
		InitNode(root->childs[i], (root->depth) + 1, case_num); /* 노드 초기화 */

		/* 선택할 수 있는 돌 중에 하나를 가져감 */
		input.push_back(caseList[i]);
		root->childs[i]->stone = caseList[i];

		double newEval = Min(root->childs[i], input, stoneNum, alpha, beta); /* Min 함수호출로 값을 구해서 */
		root->childs[i]->evaluation = newEval; /* 노드에 저장 */

		input.pop_back(); /* 선택할 수 있는 돌 중에 가져간 돌을 다시 내려놓음 */

		if (newEval > alpha) {
			alpha = newEval;
			root->evaluation = alpha;
		}
	}

	root->childNum = case_num;
	return alpha; /* alpha 값 리턴 */
}

/**
함 수 : Min(Node* root, vector<int> input, int stoneNum, double alpha, double beta)
기 능 : 현재 자식 노드들의 Evaluation 값 중 제일 작은 값으로 저장하는 함수
*/
double Min(Node* root, vector<int> input, int stoneNum, double alpha, double beta) {
	vector<int> caseList;
	int case_num = 0;

	if (root->depth > maxdepth)
		maxdepth = root->depth;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 다음 차례에 가져갈 수 있는 돌이 없는 경우 */
	if (case_num == 0) {
		return evaluate_Min(input, stoneNum); /* 현재 돌의 Evaluation 계산 */
	}

	/* 가능한 수만큼 alpha-beta 알고리즘을 사용하여 수를 계산 */
	for (int i = 0; i < case_num; i++) {

		/* beta-cut */
		if (alpha >= beta)
			continue;

		root->childs.push_back(new Node); /* 새로운 노드 생성 */
		InitNode(root->childs[i], (root->depth) + 1, case_num); /* 노드 초기화 */

		/* 선택할 수 있는 돌 중에 하나를 가져감 */
		input.push_back(caseList[i]);
		root->childs[i]->stone = caseList[i];

		double newEval = Max(root->childs[i], input, stoneNum, alpha, beta); /* Max 함수호출로 값을 구해서 */
		root->childs[i]->evaluation = newEval; /* 노드에 저장 */

		input.pop_back(); /* 선택할 수 있는 돌 중에 가져간 돌을 다시 내려놓음 */

		if (newEval < beta) {
			beta = newEval;
			root->evaluation = beta;
		}
	}

	root->childNum = case_num;
	return beta; /* beta 값 리턴 */
}

/**
함 수 : evaluate_Max(vector<int> input, int stoneNum)
기 능 : 규칙에 따라 현재 돌의 Evaluation 계산
*/
double evaluate_Max(vector<int> input, int stoneNum) {
	vector<int> caseList;
	int case_num = 0;
	double evaluate_max = 0;
	int TF = 0, decimal = 0, takeNum_one = 0, takeNum_decimal = 0, cnt = 0;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 가져간 돌 리스트에 1이 있는지 판별 */
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == 1) {
			TF = 1;
		}
	}

	/* 소수 판별 */
	for (int i = 2; i < input.back(); i++) {
		if (input.back() % i == 0) {
			decimal = 1;
			break;
		}
	}

	/* 마지막에 1 가져간 경우, 다음 차례에 가져갈 수 있는 돌 개수 계산 */
	takeNum_one = stoneNum - input.size();

	/* 마지막에 소수 가져간 경우, 소수 배수 개수 계산 */
	for (int i = 1; i * input.back() < stoneNum; i++) {
		takeNum_decimal++;
	}

	/* 마지막에 소수가 아닌 수 가져간 경우, 소수 개수 계산 */
	cnt = count_decimal(stoneNum); /* 소수의 개수 계산 */

	/* Max 승 */
	if (case_num == 0) {
		evaluate_max = 1.0;
	}

	else {
		/* 1번 돌을 아직 가져가지 않은 경우 */
		if (TF != 1) {
			evaluate_max = 0;
		}

		else {
			/* 마지막에 1을 가져간 경우 */
			if (input.back() == 1) {
				if ((takeNum_one % 2) != 0) {
					evaluate_max = 0.5;
				}
				else {
					evaluate_max = -0.5;
				}
			}

			/* 마지막에 소수인 돌을 가져간 경우 */
			else if (decimal == 1) {

				if ((takeNum_decimal % 2) != 0) {
					evaluate_max = 0.7;
				}
				else {
					evaluate_max = -0.7;
				}
			}

			/* 마지막에 소수가 아닌 돌을 가져간 경우 */
			else {
				if ((cnt % 2) != 0) {
					evaluate_max = 0.6;
				}
				else {
					evaluate_max = -0.6;
				}
			}
		}
	}

	return evaluate_max; /* Evaluation 리턴 */
}

/**
함 수 : evaluate_Min(vector<int> input, int stoneNum)
기 능 : 규칙에 따라 현재 돌의 Evaluation 계산
*/
double evaluate_Min(vector<int> input, int stoneNum) {
	vector<int> caseList;
	int case_num = 0;
	double evaluate_min = 0;
	int TF = 0, decimal = 0, takeNum_one = 0, takeNum_decimal = 0, cnt = 0;

	case_num = getNumOfPossibleCase(caseList, input, stoneNum); /* 다음 차례에 가져갈 수 있는 돌의 개수 계산 */

	/* 가져간 돌 리스트에 1이 있는지 판별 */
	for (unsigned int i = 0; i < input.size(); i++) {
		if (input[i] == 1) {
			TF = 1;
		}
	}

	/* 소수 판별 */
	for (int i = 2; i < input.back(); i++) {
		if (input.back() % i == 0) {
			decimal = 1;
			break;
		}
	}

	/* 마지막에 1 가져간 경우, 다음 차례에 가져갈 수 있는 돌 개수 계산 */
	takeNum_one = stoneNum - input.size();

	/* 마지막에 소수 가져간 경우, 소수 배수 개수 계산 */
	for (int i = 1; i * input.back() < stoneNum; i++) {
		takeNum_decimal++;
	}

	/* 마지막에 소수가 아닌 수 가져간 경우, 소수 개수 계산 */
	cnt = count_decimal(stoneNum); /* 소수의 개수 계산 */

	/* Min 승 */
	if (case_num == 0) {
		evaluate_min = -1.0;
	}

	else {
		/* 1번 돌을 아직 가져가지 않은 경우 */
		if (TF != 1) {
			evaluate_min = 0;
		}

		else {
			/* 마지막에 1을 가져간 경우 */
			if (input.back() == 1) {
				if ((takeNum_one % 2) != 0) {
					evaluate_min = -0.5;
				}
				else {
					evaluate_min = 0.5;
				}
			}

			/* 마지막에 소수 가져간 경우, 소수 배수 개수 계산 */
			else if (decimal == 1) {

				if ((takeNum_decimal % 2) != 0) {
					evaluate_min = -0.7;
				}
				else {
					evaluate_min = 0.7;
				}
			}

			/* 마지막에 소수가 아닌 돌을 가져간 경우 */
			else {
				if ((cnt % 2) != 0) {
					evaluate_min = -0.6;
				}
				else {
					evaluate_min = 0.6;
				}
			}
		}
	}

	return evaluate_min; /* Evaluation 리턴 */
}

/**
함 수 : count_decimal(int stoneNum)
기 능 : 소수의 개수 계산
*/
int count_decimal(int stoneNum) {
	int cnt1, cnt2 = 0;

	/* 전체 돌개수까지 판단 */
	for (int i = 2; i <= stoneNum; i++) {
		cnt1 = 0; /* 판단 값은 항상 초기화 */

		/* 해당 숫자까지 나누기 */
		for (int j = 1; j <= i; j++) {

			/* 이 숫자가 0으로 나눠질 경우 */
			if (i % j == 0)
				cnt1++; /* 판단 값을 증가 */
		}

		/* 판단 값이 2일 경우 */
		if (cnt1 == 2)
			cnt2++; /* 최종 리턴값 증가 */
	}

	return cnt2;
}

/**
함 수 : PrintOutput(int bestMove, Node* root, vector<int> input)
기 능 : Output 출력
*/
void PrintOutput(int bestMove, Node* root, vector<int> input) {

	printf("Best Move : %d\n", bestMove);
	printf("Calculated Value : %lf\n", root->childs[0]->evaluation);
	printf("Number of Visited Nodes : %d\n", input.size());
	printf("Max Depth : %d\n", maxdepth);
}
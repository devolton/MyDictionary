#pragma once
#include <iostream> 
#include "Node.h"
template <typename TKey, typename TData>
class Dictionary {
	typedef Node<TKey, TData> Node;
	typedef std::pair<TKey, TData> pair;
	Node* root = nullptr;
public:
	Dictionary() {}
	Dictionary(const Dictionary& other) {
		if (other.root == nullptr) return;
		this->root = new Node(other.root->key, other.root->data);
	}
	void Insert(pair pair) {
		root = Insert(false, root, pair);
	}
	void Insert_or_assign(pair pair) {
		root = Insert(true, root, pair);
	}
	void InOrder() {
		InOrder(root);
		std::cout << std::endl;
	}

	void PreOrder() {
		PreOrder(root);
		std::cout << std::endl;
	}
	void PostOrder() {
		PostOrder(root);
		std::cout << std::endl;
	}

	void Erase(const TKey& key) {
		root = Erase(root, key);
	}

	void Clear() {
		Clear(root);
		root = nullptr;
	}
	bool Empty() {
		return root == nullptr;
	}

	TKey& GetRoot() {
		return root->key;
	}

	TData& At(const TKey& key) {
		if (Find(root, key) != nullptr) return Find(root, key)->data;
		else throw std::exception("Key not found!");
	}

	TData& operator [](const TKey& key) {
		Node* node = Find(root, key);
		if (node != nullptr) return node->data;
		else throw std::exception("Key not found!");

	}

	~Dictionary() noexcept {
		Clear(root);
	}
private:
	Node* Insert(bool isInsert_or_assign, Node* node, pair pair) {
		if (node == nullptr) node = new Node(pair.first, pair.second);
		else if (node->key == pair.first && isInsert_or_assign) node->data = pair.second;
		else if (pair.first > node->key) node->pRight = Insert(isInsert_or_assign, node->pRight, pair);
		else if (pair.first < node->key) node->pLeft = Insert(isInsert_or_assign, node->pLeft, pair);
		UpdateHeight(node);
		Balance(node);
		return node;
	}

	void InOrder(Node* node) {
		if (node == nullptr) return;
		InOrder(node->pLeft);
		std::cout << node->key << " = " << node->data << std::endl;
		InOrder(node->pRight);
	}

	void PreOrder(Node* node) {
		if (node == nullptr) return;
		std::cout << node->key << " = " << node->data << std::endl;
		PreOrder(node->pLeft);
		PreOrder(node->pRight);
	}


	void PostOrder(Node* node) {
		if (node == nullptr) return;
		PostOrder(node->pLeft);
		PostOrder(node->pRight);
		std::cout << node->key << " = " << node->data << std::endl;
	}
	Node* GetMaxKey(Node* node) {
		if (node == nullptr) return node;
		else if (node->pRight == nullptr) return node;
		return GetMaxKey(node->pRight);
	}

	Node* GetMinKey(Node* node) {
		if (node == nullptr) return node;
		else if (node->pLeft == nullptr) return node;
		return GetMinKey(node->pLeft);
	}



	void Clear(Node* node) {
		if (node == nullptr) return;
		Clear(node->pLeft);
		Clear(node->pRight);
		delete node;
	}

	Node* Erase(Node* node, const TKey& key) {
		if (node == nullptr) return node;
		else if (key > node->key) node->pRight = Erase(node->pRight, key);
		else if (key < node->key) node->pLeft = Erase(node->pLeft, key);
		else {
			if (node->pLeft == nullptr && node->pRight == nullptr) {
				delete node;
				node = nullptr;
			}
			else if (node->pLeft == nullptr) {
				Node* temp = node->pRight;
				delete node;
				node = temp;
			}
			else if (node->pRight == nullptr) {
				Node* temp = node->pLeft;
				delete node;
				node = temp;
			}
			else {
				if (node->pRight == nullptr) {
					Node* maxInLeftBranch = GetMaxKey(node->pLeft);
					node->key = maxInLeftBranch->key;
					node->data = maxInLeftBranch->data;
					node->pLeft = Erase(node->pLeft, maxInLeftBranch->key);
				}
				else {
					Node* minInRightBranch = GetMinKey(node->pRight);
					node->key = minInRightBranch->key;
					node->data = minInRightBranch->data;
					node->pRight = Erase(node->pRight, minInRightBranch->key);
				}


			}



		}
		if (node != nullptr) {
			UpdateHeight(node);
			Balance(node);
		}
		return node;
	}

	int GetHeight(Node* node) {
		return (node == nullptr) ? -1 : node->height;
	}

	void UpdateHeight(Node* node) {
		node->height = std::max(GetHeight(node->pLeft), GetHeight(node->pRight)) + 1;
	}

	int GetBalance(Node* node) {
		return (node == nullptr) ? 0 : GetHeight(node->pRight) - GetHeight(node->pLeft);
	}

	void NodeSwap(Node* firstNode, Node* secondNode) {
		std::swap(firstNode->key, secondNode->key);
		std::swap(firstNode->data, secondNode->data);

	}

	void RightRotate(Node* node) {
		NodeSwap(node, node->pLeft);
		Node* boofer = node->pRight;
		node->pRight = node->pLeft;
		node->pLeft = node->pRight->pLeft;
		node->pRight->pLeft = node->pRight->pRight;
		node->pRight->pRight = boofer;
		UpdateHeight(node->pRight);
		UpdateHeight(node);
	}

	void LeftRotate(Node* node) {
		NodeSwap(node, node->pRight);
		Node* boofer = node->pLeft;
		node->pLeft = node->pRight;
		node->pRight = node->pLeft->pRight;
		node->pLeft->pRight = node->pLeft->pLeft;
		node->pLeft->pLeft = boofer;
		UpdateHeight(node->pLeft);
		UpdateHeight(node);
	}

	void Balance(Node* node) {
		int balance = GetBalance(node);
		if (balance == -2) {
			if (GetBalance(node->pLeft) == 1) LeftRotate(node->pLeft);
			RightRotate(node);
		}
		else if (balance == 2) {
			if (GetBalance(node->pRight) == -1) RightRotate(node->pRight);
			LeftRotate(node);
		}
	}


	Node* Find(Node* node, const TKey& key) {
		if (node != nullptr && key == node->key) return node;
		else if (key > node->key) return Find(node->pRight, key);
		else if (key < node->key) return Find(node->pLeft, key);
	}

	Node* CopyTree(Node* node) {
		if (node == nullptr) return node;
		Node* newNode = new Node(node->key, node->data);
		newNode->pLeft=CopyTree(node->pLeft);
		newNode->pRight = CopyTree(node->pRight);
		return newNode;
	}
};

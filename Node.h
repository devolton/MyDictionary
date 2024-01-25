#pragma once
template<typename TKey, typename TData>
class Node {
public:
	TKey key;
	TData data;
	Node* pRight;
	Node* pLeft;
	int height;

	explicit Node (const TKey& key, const TData& data) : key(key), data(data), pLeft(nullptr), pRight(nullptr),height(0) {};
};
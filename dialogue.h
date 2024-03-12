#pragma once

#include <string>
#include <vector>
#include <memory>

class DialogueNode;

class DialogueOption
{
public:
	int returnCode = 0;
	std::string text;
	std::shared_ptr<DialogueNode> nextNode;

	DialogueOption(std::string Text, int ReturnCode, std::shared_ptr<DialogueNode> NextNode);
};

class DialogueNode
{
public:
	std::string text;
	std::vector<std::shared_ptr<DialogueOption>> dialogueOptions;

	DialogueNode(std::string Text);
};

class DialogueTree
{
public:
	DialogueTree() {};
	~DialogueTree() {};

	int performDialogue(int posX, int posY);
	void init();

private:
	std::vector<std::shared_ptr<DialogueNode>> dialogueNodes;
	std::shared_ptr<DialogueNode> currentNode;
};
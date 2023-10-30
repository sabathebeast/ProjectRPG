#pragma once

#include <string>
#include <vector>
#include <memory>

class DialogueNode;

class DialogueOption
{
public:
	DialogueOption(std::string Text, int ReturnCode, std::shared_ptr<DialogueNode> NextNode);
	std::string text;
	int returnCode = 0;
	std::shared_ptr<DialogueNode> nextNode;
	
};

class DialogueNode
{
public:
	DialogueNode(std::string Text);
	std::string text;
	std::vector<std::shared_ptr<DialogueOption>> dialogueOptions;
};

class DialogueTree
{
public:
	DialogueTree();
	void init();
	int performDialogue(int posX, int posY);
private:
	std::vector<std::shared_ptr<DialogueNode>> dialogueNodes;
	std::shared_ptr<DialogueNode> currentNode;
};
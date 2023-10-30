#include "dialogue.h"
#include <memory>
#include "raylib.h"

DialogueNode::DialogueNode(std::string Text)
{
	text = Text;
}

DialogueOption::DialogueOption(std::string Text, int ReturnCode, std::shared_ptr<DialogueNode> NextNode)
{
	text = Text;
	returnCode = ReturnCode;
	nextNode = NextNode;
}

DialogueTree::DialogueTree()
{
}

void DialogueTree::init()
{
	std::shared_ptr< DialogueNode> node0 = std::make_shared<DialogueNode>("Hello wonderer!");
	std::shared_ptr< DialogueNode> node1 = std::make_shared<DialogueNode>("I don't want to talk to you.");
	std::shared_ptr< DialogueNode> node2 = std::make_shared<DialogueNode>("I have a quest for you!");
	std::shared_ptr< DialogueNode> node3 = std::make_shared<DialogueNode>("You will get 5 gold you greedy swine.");
	std::shared_ptr< DialogueNode> node4 = std::make_shared<DialogueNode>("Collect ten wood sticks");
	std::shared_ptr< DialogueNode> node5 = std::make_shared<DialogueNode>("Have you collected them wood sticks?");
	std::shared_ptr< DialogueNode> node6 = std::make_shared<DialogueNode>("Good job wonderer! Here's your pay.");

	// node 0
	node0->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Sup noob?", 0, node1));
	node0->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F2] Hello strange voice!", 0, node2));
	dialogueNodes.push_back(node0);

	// node 1
	node1->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Huh?", 0, node0));
	dialogueNodes.push_back(node1);

	// node 2
	node2->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] What is it?", 0, node4));
	node2->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F2] What's the pay?", 0, node3));
	dialogueNodes.push_back(node2);

	// node 3
	node3->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Okay what is it?", 0, node4));
	node3->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F2] That's sucks I'm out.", 6, nullptr));
	dialogueNodes.push_back(node3);

	// node 4
	node4->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Let's do it!", 5, node5));
	node4->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F2] No way.", 2, nullptr));
	dialogueNodes.push_back(node4);

	// node 5
	node5->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Yes I did, take them!", 4, node6));
	dialogueNodes.push_back(node5);

	// node 6
	node6->dialogueOptions.push_back(std::make_shared<DialogueOption>("[F1] Thank you!", 1, nullptr));
	dialogueNodes.push_back(node6);

	if (!dialogueNodes.empty())
	{
		currentNode = dialogueNodes[0];
	}
}

int DialogueTree::performDialogue(int posX, int posY)
{
	if (dialogueNodes.empty())
	{
		return -1;
	}

	int textSize = MeasureText(currentNode->text.c_str(), 26);

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++)
	{
		int dialogueTextSize = MeasureText(currentNode->dialogueOptions[i]->text.c_str(), 21);

		if (dialogueTextSize > textSize)
		{
			textSize = dialogueTextSize;
		}
	}

	int dialogueSize = 0;

	if (currentNode->dialogueOptions.size() == 1)
	{
		dialogueSize = 2;
	}
	else if (currentNode->dialogueOptions.size() == 2)
	{
		dialogueSize = 3;
	}
	else if (currentNode->dialogueOptions.size() == 3)
	{
		dialogueSize = 4;
	}

	DrawRectangle(posX, posY - (dialogueSize * 25) - 2, textSize, (dialogueSize * 25) + 5, GRAY);
	DrawText(currentNode->text.c_str(), posX + 5, posY - (dialogueSize * 25) + 5, 25, BLACK);

	for (int i = 0; i < currentNode->dialogueOptions.size(); i++)
	{
		std::string dialogueOptionText = currentNode->dialogueOptions[i]->text;
		DrawText(dialogueOptionText.c_str(), posX + 5, posY - (dialogueSize * 25) + 5 + 25 + (i * 25), 20, DARKBLUE);
	}

	int input = 0;

	if (IsKeyPressed(KEY_F1))
	{
		input = 1;
	}
	if (IsKeyPressed(KEY_F2) && currentNode->dialogueOptions.size() > 1)
	{
		input = 2;
	}

	if (input > 0)
	{
		if (currentNode->dialogueOptions[input - 1]->nextNode == nullptr)
		{
			return currentNode->dialogueOptions[input - 1]->returnCode;
		}

		if (currentNode->dialogueOptions[input - 1]->returnCode == 5)
		{
			int value = currentNode->dialogueOptions[input - 1]->returnCode;
			currentNode = currentNode->dialogueOptions[input - 1]->nextNode;
			return value;
		}

		currentNode = currentNode->dialogueOptions[input - 1]->nextNode;
	}

	return 0;
}
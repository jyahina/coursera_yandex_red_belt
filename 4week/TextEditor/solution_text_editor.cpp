#include "profile.h"
#include <deque>
#include <list>
#include <vector>
#include <algorithm>
#include "test_runner.h"

using namespace std;

class Editor
{
public:
	Editor() 
	{
		pointer = text.begin();
	};

	// �������� ������ �����
	void Left() 
	{
		if (pointer != text.begin())
			--pointer;
	};

	//�������� ������ ������
	void Right() 
	{
		if (pointer != text.end())
			++pointer;
	};

	//�������� ������ token
	void Insert(char token) 
	{
		text.insert(pointer, token);
	};

	//����������� �� ����� tokens ��������, ������� � ������� ������� ������� 
	void Copy(size_t tokens) 
	{
		buffer.assign(pointer, getPointer(tokens));
	};

	//�������� �� ����� tokens ��������, ������� � ������� ������� �������
	void Cut(size_t tokens) 
	{
		auto startCut = pointer;
		auto endCut = getPointer(tokens);
		advance(pointer, getGoodTokens(tokens));

		buffer.assign(startCut, endCut);
		text.erase(startCut, endCut);
	};

	// �������� ���������� ������ � ������� ������� �������
	void Paste() 
	{
		text.insert(pointer, buffer.begin(), buffer.end());
	};

	//�������� ������� ���������� ��������� ��������� 
	string GetText() const 
	{ 
		return { text.begin(), text.end() };
	};

private:
	list<char> text;
	list<char> buffer;
	list<char>::iterator pointer;

	list<char>::iterator getPointer(size_t tokens)
	{
		list<char>::iterator newPointer = pointer;
	
		advance(newPointer, getGoodTokens(tokens));

		return newPointer;
	}

	size_t getGoodTokens(size_t tokens)
	{
		return min(tokens, static_cast<size_t>(distance(pointer, text.end())));;
	}
};

void TypeText(Editor& editor, const string& text) {
	for (char c : text) {
		editor.Insert(c);
	}
}

void TestEditing() {
	{
		Editor editor;

		const size_t text_len = 12;
		const size_t first_part_len = 7;
		TypeText(editor, "hello, world");
		for (size_t i = 0; i < text_len; ++i) {
			editor.Left();
		}
		editor.Cut(first_part_len);
		for (size_t i = 0; i < text_len - first_part_len; ++i) {
			editor.Right();
		}
		TypeText(editor, ", ");
		editor.Paste();
		editor.Left();
		editor.Left();
		editor.Cut(3);

		ASSERT_EQUAL(editor.GetText(), "world, hello");
	}
	{
		Editor editor;

		TypeText(editor, "misprnit");
		editor.Left();
		editor.Left();
		editor.Left();
		editor.Cut(1);
		editor.Right();
		editor.Paste();

		ASSERT_EQUAL(editor.GetText(), "misprint");
	}
}

void TestReverse() {
	Editor editor;

	const string text = "esreveR";
	for (char c : text) {
		editor.Insert(c);
		editor.Left();
	}

	ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
	Editor editor;
	ASSERT_EQUAL(editor.GetText(), "");

	editor.Left();
	editor.Left();
	editor.Right();
	editor.Right();
	editor.Copy(0);
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
	Editor editor;

	editor.Paste();
	TypeText(editor, "example");
	editor.Left();
	editor.Left();
	editor.Paste();
	editor.Right();
	editor.Paste();
	editor.Copy(0);
	editor.Paste();
	editor.Left();
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example");
}


int main()
{
	Editor editor;

	const string text = "hello, world";
	for (char c : text) {
		editor.Insert(c);
	}

	// ������� ��������� ���������: `hello, world|`
	for (size_t i = 0; i < text.size(); ++i) {
		editor.Left();
	}
	// ������� ��������� ���������: `|hello, world`
	editor.Cut(7);

	// ������� ��������� ���������: `|world`
	// � ������ ������ ��������� ����� `hello, `
	for (size_t i = 0; i < 5; ++i) {
		editor.Right();
	}
	// ������� ��������� ���������: `world|`
	editor.Insert(',');
	editor.Insert(' ');
	// ������� ��������� ���������: `world, |`
	editor.Paste();
	// ������� ��������� ���������: `world, hello, |`
	editor.Left();
	editor.Left();

	//������� ��������� ���������: `world, hello|, `
	editor.Cut(3); // ����� �������� 2 �������
	// ������� ��������� ���������: `world, hello|`

	cout << editor.GetText();

	TestRunner tr;
	RUN_TEST(tr, TestEditing);
	RUN_TEST(tr, TestReverse);
	RUN_TEST(tr, TestNoText);
	RUN_TEST(tr, TestEmptyBuffer);

	return 0;
}

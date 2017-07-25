#pragma once
#include <string>
#include <vector>

class Label
{
public:
	class Element
	{
	public:
		std::string name;
		int nlen;
		unsigned short address;
		
		Element();
		~Element();

		void Set(const std::string& n, int len, unsigned short addr);
		void PrintInfo(FILE* fp, int maxlen);
	};
	enum AnnotationLabel
	{
		AL_Null = 0x0,
		AL_Breakpoint = 0x1,
		AL_Monitor = 0x2,
	};
	class AnnotationStatus
	{
	public:
		int curAddr, annotation;

		AnnotationStatus();

		AnnotationLabel CheckAnnotationLabel(const std::string& p, int len);

		bool AddAnnotation(const std::string& p, int len, unsigned short addr);
	} annotation;

#define MAX_LABEL_COUNT	1000
	int count, maxLabelLength;
    std::vector<Element> element;
	Label();

	Element* AddLabel(const std::string& n, int len, unsigned short addr);

	Element* GetLabel(const std::string& p, int len);
	Element* GetLabel(int addr);

	void PrintLabels(FILE* fp);
};
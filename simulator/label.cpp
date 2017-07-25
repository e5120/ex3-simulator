#include "label.h"

Label::Element::Element() : name(), nlen(0), address(0)
{
}

Label::Element::~Element()
{
}

void Label::Element::Set(const std::string& n, int len, unsigned short addr)
{
	nlen = len;
	name = n.substr(0,len);
	address = addr;
}

void Label::Element::PrintInfo(FILE * fp, int maxlen)
{
	if (fp)
    {
        fprintf(fp, "label(%*s) : %5d(0x%04x)\n", maxlen, name.c_str(), address, address);
    }
}

Label::AnnotationStatus::AnnotationStatus() : curAddr(0), annotation(AL_Null)
{
}

Label::AnnotationStatus::~AnnotationStatus()
{
}

Label::AnnotationLabel Label::AnnotationStatus::CheckAnnotationLabel(const std::string& p, int len)
{
	if (len != 3 || p[0] != '_' || p[2] != '_')
	{
		return AL_Null;
	}
	switch (p[1])
	{
	case 'B': return AL_Breakpoint;
	case 'M': return AL_Monitor;
	default: return AL_Null;
	}
}

bool Label::AnnotationStatus::AddAnnotation(const std::string& p, int len, unsigned short addr)
{
	if (curAddr != addr)
	{
		annotation = AL_Null;
        curAddr = addr;
	}
	AnnotationLabel al = CheckAnnotationLabel(p, len);
	annotation |= al;
	return (al != AL_Null);
}

Label::Label() : count(0), maxLabelLength(0), element(MAX_LABEL_COUNT)
{}

Label::~Label()
{
}

Label::Element* Label::AddLabel(const std::string& n, int len, unsigned short addr)
{
	Element* lb;
	if (count >= MAX_LABEL_COUNT)
	{
		printf("ERROR: labelCount exceeds limit %d\n", MAX_LABEL_COUNT);
        return nullptr;
	}
	if ((lb = GetLabel(n, len)))
	{
		printf("ERROR: label(%s) already exists!!\n", lb->name.c_str());
        return nullptr;
	}
	lb = &element[count++];
	lb->Set(n, len, addr);
	if (maxLabelLength < len)
	{
		maxLabelLength = len;
	}
	return lb;
}

Label::Element* Label::GetLabel(const std::string& p, int len)
{
	for (int i = 0; i < count; ++i)
	{
		if(element[i].nlen == len && element[i].name.substr(0,len) == p.substr(0,len))
        {
			return &element[i];
		}
	}
	return nullptr;
}

Label::Element* Label::GetLabel(int addr)
{
	for (int i = 0; i < count; ++i)
	{
		if (element[i].address == addr)
        {
            return &element[i];
        }
	}
	return nullptr;
}

void Label::PrintLabels(FILE* fp)
{
	if (fp)
	{
		for (int i = 0; i < count; ++i)
		{
			element[i].PrintInfo(fp, maxLabelLength);
		}
	}
}
//
//  MarkdownAST.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 31.08.22.
//

#ifndef MarkdownAST_h
#define MarkdownAST_h

#include "MarkdownCode.hpp"

namespace jp {
template<typename T> class ASTNode
{
public:
    ASTNode() {}
    
    virtual T render(RendererPtr<T> renderer) = 0;
    virtual ~ASTNode() {}
    
    virtual void dump(std::ostream& os) const = 0;
    
    // and how to format it to cout
    friend std::ostream& operator << (std::ostream& os, const ASTNode& s) {
        s.dump(os);
        return os;
    }
};

template<typename T> using ASTNodePtr = ASTNode<T> *;

template <typename T> class ASTContainer : public ASTNode<T> {
    std::vector<ASTNodePtr<T>> children;
    
public:
    ASTContainer(const ASTNodePtr<T> &child1) {children.push_back(child1);}
    ASTContainer(const ASTNodePtr<T> &child1, const ASTNodePtr<T> &child2) {children.push_back(child1);children.push_back(child2);}
    ASTContainer(const ASTNodePtr<T> &child1, const ASTNodePtr<T> &child2, const ASTNodePtr<T> &child3)
{children.push_back(child1);children.push_back(child2);children.push_back(child3);}
    ASTContainer(const ASTNodePtr<T> &child1, const ASTNodePtr<T> &child2, const ASTNodePtr<T> &child3, const ASTNodePtr<T> &child4) {children.push_back(child1);children.push_back(child2);children.push_back(child3);children.push_back(child4);}

    ~ASTContainer() override {for (auto child : children) {delete child;}}
    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {std::vector<T> renderedChildren; for (auto child : children) {renderedChildren.push_back(child->render(renderer));} return renderer->chain(renderedChildren);}
    void dump(std::ostream& os) const override {
        os << "[ASTContainer ";
        for (auto child : children) {
            os << *child << ",";
        }
        os << "] ";
    }
};

template <typename T> class ASTHeadline : public ASTNode<T> {
    ASTNodePtr<T> child;
    int level = 1;
    
public:
    ASTHeadline(const ASTNodePtr<T> &child, const std::vector<char> &headlineLevel) : child(child), level((int)headlineLevel.size()) {}
    ~ASTHeadline() override {delete child;}

    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->headline(child->render(renderer), level);}
    void dump(std::ostream& os) const override {
        os << "[ASTHeadline(" << level << ") " << *child << "] ";
    }
};

template <typename T> class ASTAttribute : public ASTNode<T> {
    ASTNodePtr<T> child;
    enum MarkdownAttribute attribute;

public:
    ASTAttribute(const ASTNodePtr<T> &child, enum MarkdownAttribute attribute) : child(child), attribute(attribute) {}
    ~ASTAttribute() override {delete child;}

    std::string getAttributeName() const {
        switch (attribute) {
            case Bold:
                return "Bold";
            case Italic:
                return "Italic";
            case BoldItalic:
                return "BoldItalic";
            case Strikethrough:
                return "Strikethrough";
            case Underline:
                return "Underline";
            default:
                return "unknown";
        }
    }
    
    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->attribute(child->render(renderer), attribute);}
    void dump(std::ostream& os) const override {
        os << "[ASTAttribute(" << getAttributeName() << ") " << *child << "] ";
    }
};

template <typename T> class ASTCode : public ASTNode<T> {
    ASTNodePtr<T> code;
    enum CodeType type;
    
public:
    ASTCode(const ASTNodePtr<T> &code, enum CodeType type = Normal) : code(code), type(type) {}
    ~ASTCode() override {delete code;}
    
    std::string getTypeName() const {
        switch (type) {
            case Normal:
                return "Normal";
            case Inline:
                return "Inline";
            case Include:
                return "Include";
            default:
                return "unknown";
        }
    }

    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->code(code->render(renderer), type);}
    void dump(std::ostream& os) const override {
        os << "[ASTCode(" << getTypeName() << ") " << *code << "] ";
    }
};

template <typename T> class ASTText : public ASTNode<T> {
    std::string text;
public:
    ASTText(std::string text) : text(text) {}
    ASTText(const std::vector<char> &charArray) {std::string s(charArray.begin(), charArray.end()); text = s;}

    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->text(text);}
    void dump(std::ostream& os) const override {
        os << std::quoted(text);
    }
};

template <typename T> class ASTNewline : public ASTNode<T> {
public:
    ASTNewline() {}

    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->newLine();};
    void dump(std::ostream& os) const override {
        os << "[ASTNewline]";
    }
};

template <typename T> class ASTLink : public ASTNode<T> {
    ASTNodePtr<T> content;
    ASTNodePtr<T> link;
protected:
    ASTNodePtr<T> getLink() const {return link == nullptr ? content : link;}
public:
    ASTLink(const ASTNodePtr<T> &content, const ASTNodePtr<T> &link) : content(content), link(link) {}
    ~ASTLink() override {delete content; if (link != nullptr) {delete link;}}
    
    
    T render(std::shared_ptr<MarkdownRenderer<T>> renderer) override {return renderer->link(content->render(renderer), getLink()->render(renderer));}
    void dump(std::ostream& os) const override {
        os << "[ASTLink " << *content << " -> " << (*getLink()) << "] ";
    }
};
}

#endif /* MarkdownAST_h */

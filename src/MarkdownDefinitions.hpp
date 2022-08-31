//
//  MarkdownDefinitions.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 31.08.22.
//

#ifndef MarkdownDefinitions_h
#define MarkdownDefinitions_h

namespace jp {

template <typename T> class MarkdownRenderer;

template <typename T>
using RendererPtr = std::shared_ptr<MarkdownRenderer<T>>;

}

#endif /* MarkdownDefinitions_h */

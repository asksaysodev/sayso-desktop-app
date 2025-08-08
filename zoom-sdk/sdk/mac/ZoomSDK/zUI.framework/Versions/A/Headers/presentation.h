#ifndef _ZUI_PRESENTATION_H_
#define _ZUI_PRESENTATION_H_

#include <memory>
#include "zui_data_define.h"

namespace zui
{
	class PresentationNode;
	typedef std::shared_ptr<PresentationNode> PresentationNodePtr;

	class ZUI_API Presentation
	{
	public:
		enum class Type
		{
			kUnknown,
			kTooltip,
			kPopover,
		};

		Presentation() = default;
		Presentation(PresentationNodePtr ptr)
			: _nodeptr(ptr)
		{
		}

		virtual ~Presentation() = default;

		PresentationNodePtr GetNodePtr()
		{
			if (nullptr == _nodeptr)
				_nodeptr = this->Build();
			return _nodeptr;
		}

		virtual PresentationNodePtr Build() = 0;

		virtual Presentation::Type GetType() const { return Presentation::Type::kUnknown; }

	protected:
		PresentationNodePtr _nodeptr;
	};
}

#endif // PRESENTATION_H

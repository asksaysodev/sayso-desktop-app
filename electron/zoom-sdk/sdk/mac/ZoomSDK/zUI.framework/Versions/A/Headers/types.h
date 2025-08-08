#ifndef TYPES_H
#define TYPES_H

#include <bitset>
#include <math.h>

#include "zui_data_define.h"

namespace zui
{
	enum class Result
	{
		kSuccess = 0,
		kFailure = 1
	};

	inline Result& operator|=(Result& lhs, Result rhs)
	{
		using Underlying = std::underlying_type_t<Result>;
		lhs = static_cast<Result>(
						static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs)
				);
		return lhs;
	}

  enum class WindowType
  {
      Modal,
      Borderless,
  };
}

namespace Layout ///@todo change this to zui
{
	struct Position
	{
		float x;
		float y;

		Position(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		Position()
		{
			this->x = 0;
			this->y = 0;
		}
	};

	struct Size
	{
		float width;
		float height;

		Size(float _width, float _height)
		{
			width = _width;
			height = _height;
		};

		Size()
		{
			width = NAN;
			height = NAN;
		};

		bool operator==(const Size& rhs) const;
		bool operator!=(const Size& rhs) const;
	};

	enum class Alignment
	{
		Undefined,
		Center,
		Start,
		End,
		Fill
	};

	enum class JustifyContent
	{
		Start,
		End,
		SpaceBetween,
		Center,
		SpaceAround,
		SpaceEvenly
	};

	enum class Wrap
	{
		NoWrap,
		Wrap
	};

	struct Rect
	{
		Rect() = default;

		Rect(float value)
			: left(value)
			, top(value)
			, right(value)
			, bottom(value)
		{
		}

		Rect(float left, float top, float right, float bottom)
			: left(left)
			, top(top)
			, right(right)
			, bottom(bottom)
		{
		}

		float left = 0.f;
		float top = 0.f;
		float right = 0.f;
		float bottom = 0.f;
	};

	struct EdgeInsets
	{
		float leading = 0;
		float top = 0;
		float trailing = 0;
		float bottom = 0;

		EdgeInsets() = default;

		EdgeInsets(float _leading, float _top, float _trailing, float _bottom)
			: leading(_leading), top(_top), trailing(_trailing), bottom(_bottom)
		{
		}

		bool operator==(const EdgeInsets& rhs) const;
		bool operator!=(const EdgeInsets& rhs) const;
	};

	enum class Edge
	{
		Leading,
		Top,
		Trailing,
		Bottom
	};

	struct ZUI_API EdgeSet
	{
		std::bitset<4> edges;

		EdgeSet() = default;

		EdgeSet(std::initializer_list<Edge> edgesList)
		{
			for (auto edge : edgesList)
			{
				edges.set(static_cast<size_t>(edge));
			}
		}

		bool Contains(Edge edge) const
		{
			return edges.test(static_cast<size_t>(edge));
		}

		EdgeSet operator|(const EdgeSet& other) const
		{
			EdgeSet result;
			result.edges = this->edges | other.edges;
			return result;
		}

		static const EdgeSet All;
		static const EdgeSet Top;
		static const EdgeSet Bottom;
		static const EdgeSet Leading;
		static const EdgeSet Trailing;
		static const EdgeSet Horizontal;
		static const EdgeSet Vertical;
	};

	struct ScalingFactors
	{
		float growthFactor;
		float shrinkFactor;

		ScalingFactors(float _growthFactor, float _shrinkFactor)
		{
			growthFactor = _growthFactor;
			shrinkFactor = _shrinkFactor;
		};

		ScalingFactors()
		{
			growthFactor = 0.0f;
			shrinkFactor = 0.0f;
		};

		bool operator==(const ScalingFactors& rhs) const;
		bool operator!=(const ScalingFactors& rhs) const;
	};

	enum class ContentSizing
	{
		ContentBox,
		BorderBox
	};
}

#endif // TYPES_H

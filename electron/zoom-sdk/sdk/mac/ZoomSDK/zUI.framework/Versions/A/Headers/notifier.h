#pragma once

#include "zui_data_define.h"
#include "view.h"

namespace zui
{
    class ZUI_API Notifier : public View
    {
    public:
      enum class Type
      {
        kDot,
        kCounterRed,
        kCounterRedInverse,
        kCounterBlue,
      };

      enum class Size
      {
        kDefault,
        kSmall,
        kLarge,
        kXLarge
      };

    public:
        /// @brief Construct a new Avatar View
        Notifier();

        /// @brief Virtual destructor of the Badge class.
        virtual ~Notifier() = default;

        std::shared_ptr<ViewNode> Build() override;

        Notifier &type(const Bind<Notifier::Type> &type);
        Notifier& typeDot();
        Notifier& typeCounterRed();
        Notifier& typeCounterRedInverse();
        Notifier& typeCounterBlue();


        Notifier &size(const Bind<Notifier::Size>& size);
        Notifier& sizeDefault();
        Notifier& sizeSmall();
        Notifier& sizeLarge();
        Notifier& sizeXLarge();


        /**
         * @brief Define the count displayed in the notifier.
         *
         * @param[in] count The label displayed in the badge.
         * @todo Change count to label to follow the naming convention.
         */
        Notifier &count(const Bind<unsigned int>& count);
    };
}

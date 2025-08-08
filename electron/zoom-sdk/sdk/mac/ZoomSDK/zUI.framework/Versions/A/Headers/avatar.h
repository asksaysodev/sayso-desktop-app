#pragma once

#include "zui_data_define.h"
#include <functional>
#include "view.h"

namespace zui
{
	/**
	 * @class Avatar
	 * @brief A view representing an avatar.
	 *
	 * An avatar is a graphical representation of a user or entity, typically a profile image or icon.
	 * It is commonly used in communication, messaging apps, and other platforms where users interact with each other.
	 */
	class ZUI_API Avatar
		: public View
	{
	public:

		/**
	* @enum AvatarStatus
	* @brief Defines the status of the avatar
	*/
		enum class Status
		{
			kNone,
			kAway,
			kBlock,
			kBusy,
			kDoNotDisturb,
			kInCalendarEvent,
			kInCall,
			kInMeeting,
			kMobile,
			kOffline,
			kOnline,
			kOutOfOffice
		};

		/**
	* @enum AvatarSize
	* @brief Defines the size of the avatar
	*/
		enum class Size
		{
			kSize20, // tag
			kSize24, // default
			kSize32,
			kSize40,
			kSize48,
			kSize64,
			kSize80,
			kSize110
		};

	public:
		/// @brief Construct a new Avatar object
		Avatar();

		/// @brief Virtual destructor of the Avatar class.
		virtual ~Avatar() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Register the event handler when the avatar is clicked
		 * @param[in] callback The event handler when the avatar is clicked
		 */
		Avatar& onClick(std::function<void()> callback);

		/**
		 * @brief Register the event handler when the avatar is hovered over
		 * @param[in] callback The event handler when the avatar is hovered over
		 */
		Avatar& onHover(std::function<void(bool)> callback);

		/**
		 * @brief Define the image of the avatar
		 * @param[in] avatar_img The image of the avatar
		 */
		Avatar& image(const Bind<std::string>& avatar_img);

		/**
		 * @brief Define the name of the user that the avatar represents
		 * @param user_name The name of the user that the avatar represents
		 *
		 * The name is displayed when the image is not available.
		 */
		Avatar& name(const Bind<std::string>& user_name);

		/**
		 * @brief Define the background color of the avatar if the image is not available
		 * @param[in] bkcolor
		 */
		Avatar& backgroundColor(Bind<unsigned int> bkcolor);

		/**
		 * @brief Define the presence status of the user
		 * @param[in] status The presence status of the user
		 */
		Avatar& presenceStatus(const Bind<Status>& status);

		/**
		 * @brief Define the size of the avatar
		 * @param[in] size The size of the avatar
		 */
		Avatar& avatarSize(const Bind<Size>& size);
	};
}

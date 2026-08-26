#include <sparkle>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

namespace
{
	constexpr spk::Vector2UInt WindowSize{1100, 760};

	unsigned int reduced(unsigned int value, unsigned int amount)
	{
		return value > amount ? value - amount : 0;
	}

	spk::Rect2D rectangle(int x, int y, unsigned int width, unsigned int height)
	{
		return {spk::Vector2Int{x, y}, spk::Vector2UInt{width, height}};
	}

	void configureText(spk::TextLabel &label, std::string_view text, std::size_t size)
	{
		label.setText(text);
		label.setTextSize(size);
	}

	class TextShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::DynamicTextLabel _dynamicText;
		spk::TextLabel _echo;
		spk::TextArea _textArea;
		spk::TextEdit _input;
		spk::TextEdit _password;
		spk::TextLabel _caption;
		spk::TextEdit::EditionContract _inputContract;
		spk::TextEdit::SelectionContract _selectionContract;
		std::size_t _generation = 0;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}

			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			const unsigned int halfWidth = reduced(width, 72) / 2;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_dynamicText.setGeometry(rectangle(24, 74, halfWidth, 48));
			_echo.setGeometry(rectangle(static_cast<int>(48 + halfWidth), 74, halfWidth, 48));
			_textArea.setGeometry(rectangle(24, 140, reduced(width, 48), 172));
			_input.setGeometry(rectangle(24, 336, halfWidth, 42));
			_password.setGeometry(rectangle(static_cast<int>(48 + halfWidth), 336, halfWidth, 42));
			_caption.setGeometry(rectangle(24, 398, reduced(width, 48), 40));
		}

	public:
		TextShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_dynamicText(this->name() + ".dynamic", &_surface),
			_echo(this->name() + ".echo", &_surface),
			_textArea(this->name() + ".area", &_surface),
			_input(this->name() + ".input", &_surface),
			_password(this->name() + ".password", &_surface),
			_caption(this->name() + ".caption", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Text output and input", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			configureText(_dynamicText, "", 21);
			_dynamicText.setRefreshDuration(std::chrono::milliseconds(800));
			_dynamicText.setTextProducer([this]() {
				return "Dynamic refresh #" + std::to_string(++_generation);
			});
			configureText(_echo, "Input: <empty>", 18);
			_echo.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_textArea.setTextSize(17);
			_textArea.setLinePadding(6);
			_textArea.setMinimalWidth(280);
			_textArea.setGlyphColor({0.82f, 0.87f, 0.94f, 1.0f});
			_textArea.setOutlineColor({0.03f, 0.05f, 0.08f, 1.0f});
			_textArea.setText("TextLabel renders a measured line. TextArea preserves blank lines and wraps words to its width.\n\nThe fields below demonstrate editable and obscured TextEdit content.");
			_input.setPlaceholder("Type text here...");
			_input.setSelectionColor({0.19f, 0.48f, 0.92f, 0.62f});
			_password.setPlaceholder("Obscured input...");
			_password.setObscured(true);
			_password.setText("sparkle");
			configureText(_caption, "TextEdit selection: drag, Shift+arrows, Ctrl+A/C/X/V; clicking outside clears it.", 16);
			_caption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_inputContract = _input.subscribeToEdition([this](const spk::Font::Text &) {
				_echo.setText("Input: " + (_input.hasText() ? _input.textAsUTF8() : std::string("<empty>")));
			});
			_selectionContract = _input.subscribeToSelection([this](const spk::TextEdit::Selection &selection) {
				_caption.setText(
					selection.empty() ? "TextEdit selection: drag, Shift+arrows, Ctrl+A/C/X/V; clicking outside clears it." : "TextEdit selected " + std::to_string(selection.length()) + " codepoints.");
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class InteractionShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::PushButton _pushButton;
		spk::IconButton _iconButton;
		spk::CheckableIconButton _checkableButton;
		spk::CheckBox _checkBox;
		spk::RadioButtonGroup _radioGroup;
		spk::ToggleSwitch _toggle;
		spk::Tooltip _tooltip;
		spk::SpinBox<int> _spinBox;
		spk::FloatSpinBox _numericSpinBox;
		spk::SliderBar _slider;
		spk::TextLabel _status;
		spk::PushButton::ClickContract _pushContract;
		spk::CheckableIconButton::StateContract _checkContract;
		spk::CheckBox::StateContract _checkBoxContract;
		spk::RadioButtonGroup::SelectionContract _radioContract;
		spk::ToggleSwitch::StateContract _toggleContract;
		spk::SpinBox<int>::EditionContract _spinContract;
		spk::FloatSpinBox::EditionContract _numericContract;
		std::size_t _clickCount = 0;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}

			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_pushButton.setGeometry(rectangle(30, 84, 170, 48));
			_iconButton.setGeometry(rectangle(222, 84, 52, 48));
			_checkableButton.setGeometry(rectangle(296, 84, 52, 48));
			_spinBox.setGeometry(rectangle(30, 166, 310, 44));
			_numericSpinBox.setGeometry(rectangle(370, 166, 340, 44));
			_slider.setGeometry(rectangle(30, 252, reduced(width, 60), 26));
			_status.setGeometry(rectangle(30, 310, reduced(width, 60), 44));
			_checkBox.setGeometry(rectangle(30, 370, reduced(width, 60), 34));
			_radioGroup.setGeometry(rectangle(30, 414, reduced(width, 60), 82));
			_toggle.setGeometry(rectangle(30, 510, 104, 42));
		}

		void _configureSpinButton(spk::IconButton &button)
		{
			button.setIconSize({18, 18});
		}

		void _configureCheckable(spk::CheckableIconButton &button)
		{
			button.uncheckedButton().setIconSize({18, 18});
			button.checkedButton().setIconSize({18, 18});
		}

	public:
		InteractionShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_pushButton(this->name() + ".push", &_surface),
			_iconButton(this->name() + ".icon", &_surface),
			_checkableButton(this->name() + ".checkable", &_surface),
			_checkBox(this->name() + ".check-box", &_surface),
			_radioGroup(this->name() + ".radio-group", &_surface),
			_toggle(this->name() + ".toggle", &_surface),
			_tooltip(this->name() + ".tooltip", &_surface),
			_spinBox(this->name() + ".spin", &_surface),
			_numericSpinBox(this->name() + ".numeric", &_surface),
			_slider(this->name() + ".slider", &_surface),
			_status(this->name() + ".status", &_surface)
		{
			_iconButton.setIconSpriteID(5);
			_surface.setCornerSize({12, 12});
			configureText(_title, "Buttons, state, and numeric controls", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_pushButton.setText("PushButton");
			_configureCheckable(_checkBox.indicator());
			_checkBox.setIndicatorSize({28, 28});
			_checkBox.setSpacing(10);
			_checkBox.setText("CheckBox toggles from its whole row");
			configureText(_checkBox.label(), "CheckBox toggles from its whole row", 17);
			_checkBox.label().setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_radioGroup.setSpacing(8);
			_radioGroup.setIndicatorSize({24, 24});
			_radioGroup.setElementPadding({18, 6});
			for (const auto &[column, row, text] : std::vector<std::tuple<std::size_t, std::size_t, std::string_view>>{
					 {0, 0, "Alpha"}, {1, 0, "Beta"}, {0, 1, "Gamma"}, {1, 1, "Delta"}})
			{
				spk::RadioButton &button = _radioGroup.insert(column, row, text);
				_configureCheckable(button.indicator());
				configureText(button.label(), text, 16);
				button.label().setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			}
			_toggle.setOutlineCornerSize({8, 8});
			_toggle.setThumbCornerSize({7, 7});
			_toggle.setThumbSize({30, 30});
			_toggle.setPadding({6, 6});
			_tooltip.background().setCornerSize({9, 9});
			_tooltip.textArea().setTextSize(15);
			_tooltip.textArea().setGlyphColor({0.94f, 0.96f, 1.0f, 1.0f});
			_tooltip.textArea().setOutlineColor({0.03f, 0.04f, 0.08f, 1.0f});
			_tooltip.textArea().setLinePadding(3);
			_tooltip.setText("ToggleSwitch animates one shared ratio for its thumb and split backgrounds.");
			_tooltip.setMaximumWidth(290);
			_tooltip.setTarget(&_toggle);
			_configureSpinButton(_spinBox.downButton());
			_configureSpinButton(_spinBox.upButton());
			_spinBox.setMinimum(0);
			_spinBox.setMaximum(10);
			_spinBox.setValue(4);
			_configureSpinButton(_numericSpinBox.lowerButton());
			_configureSpinButton(_numericSpinBox.raiseButton());
			_numericSpinBox.setStep(0.25f);
			_numericSpinBox.setValue(1.5f);
			_slider.setScale(0.22f);
			_slider.setRatio(0.35f);
			configureText(_status, "Interact with any control on this page.", 18);
			_status.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_pushContract = _pushButton.subscribeToClick([this]() {
				_status.setText("PushButton clicks: " + std::to_string(++_clickCount));
			});
			_checkContract = _checkableButton.subscribeToState([this](bool checked) {
				_status.setText(checked ? "CheckableIconButton: checked" : "CheckableIconButton: unchecked");
			});
			_checkBoxContract = _checkBox.subscribeToState([this](bool checked) {
				_status.setText(checked ? "CheckBox: checked" : "CheckBox: unchecked");
			});
			_radioContract = _radioGroup.subscribeToSelection([this](std::optional<spk::RadioButtonGroup::Selection> selection) {
				_status.setText(selection.has_value() ? "Radio selection: cell [" + std::to_string(selection->cell.column) + ", " + std::to_string(selection->cell.row) + "]" : "Radio selection cleared");
			});
			_toggleContract = _toggle.subscribeToState([this](bool checked) {
				_status.setText(checked ? "ToggleSwitch: on" : "ToggleSwitch: off");
			});
			_spinContract = _spinBox.subscribeToEdition([this](int value) {
				_status.setText("SpinBox value: " + std::to_string(value));
			});
			_numericContract = _numericSpinBox.subscribeToEdition([this](float value) {
				_status.setText("NumericSpinBox value: " + std::to_string(value));
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class ScrollingShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::IScrollArea _scrollArea;
		spk::ImageLabel _oversizedContent;
		spk::TextLabel _caption;
		spk::LabeledProgressBar _leftToRightProgress;
		spk::LabeledProgressBar _rightToLeftProgress;
		spk::ProgressBar _bottomToTopProgress;
		spk::ProgressBar _topToBottomProgress;
		spk::SliderBar _slider;
		spk::ScrollBar _scrollBar;
		spk::SliderBar::EditionContract _progressContract;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}

			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			const unsigned int height = _surface.geometry().height;
			const int progressY = static_cast<int>(reduced(height, 146));
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_scrollArea.setGeometry(rectangle(30, 72, reduced(width, 60), reduced(height, 290)));
			_caption.setGeometry(rectangle(30, progressY - 38, reduced(width, 60), 28));
			_leftToRightProgress.setGeometry(rectangle(50, progressY, 360, 24));
			_slider.setGeometry(rectangle(50, progressY + 36, 360, 24));
			_rightToLeftProgress.setGeometry(rectangle(50, progressY + 72, 360, 30));
			_bottomToTopProgress.setGeometry(rectangle(30, progressY, 12, 102));
			_topToBottomProgress.setGeometry(rectangle(418, progressY, 12, 102));
			_scrollBar.setGeometry(rectangle(470, progressY + 36, reduced(width, 500), 24));
		}

	public:
		ScrollingShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_scrollArea(this->name() + ".area", &_surface),
			_oversizedContent(this->name() + ".content", &_scrollArea.container()),
			_caption(this->name() + ".caption", &_surface),
			_leftToRightProgress(this->name() + ".left-to-right-progress", &_surface),
			_rightToLeftProgress(this->name() + ".right-to-left-progress", &_surface),
			_bottomToTopProgress(this->name() + ".bottom-to-top-progress", &_surface),
			_topToBottomProgress(this->name() + ".top-to-bottom-progress", &_surface),
			_slider(this->name() + ".slider", &_surface),
			_scrollBar(this->name() + ".scroll-bar", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Clipping, scrollbars, and scroll areas", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_scrollArea.setScrollBarWidth(22);
			_oversizedContent.setMinimalSize({1200.0f, 520.0f});
			_scrollArea.setContent(&_oversizedContent);
			_scrollArea.horizontalScrollBar().setRatio(0.28f);
			_scrollArea.verticalScrollBar().setRatio(0.32f);
			configureText(_caption, "Resize the window: view size, overflow, thumb scale, and content anchors remain synchronized.", 16);
			_caption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_leftToRightProgress.setCornerSize({7, 7});
			_leftToRightProgress.setRatio(0.42f);
			_leftToRightProgress.setTextPredicate([](float ratio) {
				return std::to_string(static_cast<int>(std::lround(ratio * 100.0f))) + " %";
			});
			configureText(_leftToRightProgress.label(), "", 16);
			_leftToRightProgress.refreshText();
			_rightToLeftProgress.setCornerSize({8, 8});
			_rightToLeftProgress.setRatio(0.42f);
			_rightToLeftProgress.setFillDirection(spk::ProgressBar::FillDirection::RightToLeft);
			configureText(_rightToLeftProgress.label(), "", 16);
			_rightToLeftProgress.refreshText();
			_bottomToTopProgress.setCornerSize({5, 5});
			_bottomToTopProgress.setRatio(0.42f);
			_bottomToTopProgress.setFillDirection(spk::ProgressBar::FillDirection::BottomToTop);
			_topToBottomProgress.setCornerSize({5, 5});
			_topToBottomProgress.setRatio(0.42f);
			_topToBottomProgress.setFillDirection(spk::ProgressBar::FillDirection::TopToBottom);
			_slider.setScale(0.3f);
			_slider.setRatio(0.42f);
			_scrollBar.setScale(0.3f);
			_scrollBar.setRatio(0.45f);
			_progressContract = _slider.subscribeToEdition([this](float ratio) {
				_leftToRightProgress.setRatio(ratio);
				_rightToLeftProgress.setRatio(ratio);
				_bottomToTopProgress.setRatio(ratio);
				_topToBottomProgress.setRatio(ratio);
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class WindowShowcaseContent final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _background;
		spk::TextArea _text;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_background.setGeometry(rectangle(0, 0, geometry().width, geometry().height));
			_text.setGeometry(rectangle(18, 16, reduced(geometry().width, 36), reduced(geometry().height, 32)));
		}

	public:
		explicit WindowShowcaseContent(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_background(this->name() + ".background", this),
			_text(this->name() + ".text", &_background)
		{
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}

		void configure()
		{
			_background.setCornerSize({10, 10});
			_text.setTextSize(17);
			_text.setLinePadding(5);
			_text.setGlyphColor({0.82f, 0.87f, 0.94f, 1.0f});
			_text.setOutlineColor({0.03f, 0.05f, 0.08f, 1.0f});
			_text.setAlignment({spk::Alignment::Horizontal::Center, spk::Alignment::Vertical::Center});
			_text.setText("This is a typed InterfaceWindow content widget. Drag the title, resize an edge, or use the minimize and maximize controls.");
		}
	};

	class WindowsShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::InterfaceWindow<WindowShowcaseContent> _window;
		spk::TextLabel _status;
		spk::IInterfaceWindow::CloseContract _closeContract;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			if (!_window.isMaximized())
			{
				_window.setGeometry(rectangle(90, 82, std::min(760u, reduced(width, 130)), 360));
			}
			_status.setGeometry(rectangle(30, 470, reduced(width, 60), 36));
		}

	public:
		WindowsShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_window(this->name() + ".window", &_surface),
			_status(this->name() + ".status", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Movable and scalable interface windows", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_window.setTitle("InterfaceWindow<WindowShowcaseContent>");
			_window.setMinimumContentSize({300.0f, 160.0f});
			_window.setZOrder(1.0f);
			_window.contentObject().configure();
			configureText(_status, "The close button emits a callback but does not close the generic window.", 16);
			_status.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_closeContract = _window.subscribeToClose([this]() {
				_status.setText("Close requested: the showcase keeps the generic window active.");
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class PanelsShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::TextLabel _commandCaption;
		spk::CommandPanel _commands;
		spk::PromptPanel _prompt;
		spk::TextLabel _status;
		spk::PushButton::ClickContract _newContract;
		spk::PushButton::ClickContract _saveContract;
		spk::PushButton::ClickContract _closeContract;
		spk::PushButton::ClickContract _cancelContract;
		spk::PushButton::ClickContract _confirmContract;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}

			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_commandCaption.setGeometry(rectangle(30, 76, reduced(width, 60), 28));
			_commands.setGeometry(rectangle(30, 110, reduced(width, 60), 48));
			_prompt.setGeometry(rectangle(30, 190, reduced(width, 60), 290));
			_status.setGeometry(rectangle(30, 510, reduced(width, 60), 36));
		}

	public:
		PanelsShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_commandCaption(this->name() + ".command-caption", &_surface),
			_commands(this->name() + ".commands", &_surface),
			_prompt(this->name() + ".prompt", &_surface),
			_status(this->name() + ".status", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Command rows and framed prompts", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			configureText(_commandCaption, "CommandPanel preserves insertion order and pushes compact commands to the right.", 16);
			_commandCaption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);

			_commands.setElementPadding({10, 0});
			_commands.addButton("new", "New");
			_commands.addButton("save", "Save");
			_commands.addButton("close", "Close");

			_prompt.background().setCornerSize({16, 16});
			_prompt.textArea().setTextSize(18);
			_prompt.textArea().setLinePadding(6);
			_prompt.textArea().setGlyphColor({0.92f, 0.95f, 1.0f, 1.0f});
			_prompt.textArea().setOutlineColor({0.03f, 0.04f, 0.08f, 1.0f});
			_prompt.setMessage("PromptPanel combines a wrapping TextArea with a dynamically editable CommandPanel. Resize the application to see the message reflow while the command row remains anchored at the bottom-right.");
			_prompt.setButtonPadding({10, 0});
			_prompt.addButton("cancel", "Cancel");
			_prompt.addButton("confirm", "Confirm");

			configureText(_status, "Click a command to exercise its delegated callback.", 16);
			_status.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_newContract = _commands.subscribe("new", [this]() {
				_status.setText("CommandPanel: New selected");
			});
			_saveContract = _commands.subscribe("save", [this]() {
				_status.setText("CommandPanel: Save selected");
			});
			_closeContract = _commands.subscribe("close", [this]() {
				_status.setText("CommandPanel: Close selected");
			});
			_cancelContract = _prompt.subscribe("cancel", [this]() {
				_status.setText("PromptPanel: Cancel selected");
			});
			_confirmContract = _prompt.subscribe("confirm", [this]() {
				_status.setText("PromptPanel: Confirm selected");
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class MenuWorkspaceContent final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _background;
		spk::TextModel _model{
			"Aurora",
			"Borealis",
			"Cassiopeia",
			"Draco",
			"Equinox",
			"Fomalhaut",
			"Gemini",
			"Helios"};
		spk::TextLabel _selectorCaption;
		spk::TextModel::Delegate _comboDelegate;
		spk::ComboBox _combo;
		spk::TextModel::View _list;
		spk::TextArea _message;
		spk::ComboBox::SelectionContract _comboContract;
		spk::TextModel::View::SelectionContract _listContract;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_background.setGeometry(rectangle(0, 0, geometry().width, geometry().height));
			const unsigned int selectorWidth = std::min(380u, reduced(geometry().width, 84) / 2);
			_selectorCaption.setGeometry(rectangle(28, 20, selectorWidth, 26));
			_combo.setGeometry(rectangle(28, 52, selectorWidth, 40));
			_list.setGeometry(rectangle(28, 108, selectorWidth, reduced(geometry().height, 136)));
			_message.setGeometry(rectangle(static_cast<int>(56 + selectorWidth), 28, reduced(geometry().width, selectorWidth + 84), reduced(geometry().height, 56)));
		}

	public:
		explicit MenuWorkspaceContent(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_background(this->name() + ".background", this),
			_selectorCaption(this->name() + ".selector-caption", &_background),
			_combo(this->name() + ".combo", &_background),
			_list(this->name() + ".list", &_background),
			_message(this->name() + ".message", &_background)
		{
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}

		void configure()
		{
			_background.setCornerSize({14, 14});
			configureText(_selectorCaption, "TextModel-backed ComboBox and View", 16);
			_selectorCaption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			configureText(_combo.displayLabel(), "", 17);
			_combo.displayLabel().setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_combo.displayLabel().setPadding({10, 4});
			_combo.background().setCornerSize({8, 8});
			_combo.popupBackground().setCornerSize({8, 8});
			_combo.openButton().setText(spk::Font::Text{U'\u25BE'});
			_combo.setOpenButtonWidth(42);
			auto configureDelegate = [](spk::TextModel::Delegate &delegate, bool selectedSurface) {
				delegate.setTextSize(16);
				delegate.setGlyphColor(selectedSurface ? spk::Color{0.96f, 0.98f, 1.0f, 1.0f} : spk::Color{0.80f, 0.86f, 0.94f, 1.0f});
				delegate.setOutlineColor({0.03f, 0.05f, 0.08f, 1.0f});
				delegate.setPadding({10, 4});
				delegate.setRowHeight(30);
			};
			configureDelegate(_comboDelegate, true);
			_combo.setItemDelegate(&_comboDelegate);
			_combo.setPlaceholder("Choose a constellation...");
			_combo.setMaximumVisibleRows(5);
			_combo.setModel(&_model);
			_combo.setSelectedRow(0);
			configureDelegate(_list.defaultDelegate(), false);
			_list.setModel(&_model);
			_list.setSelectedRow(2);
			_message.setTextSize(20);
			_message.setLinePadding(6);
			_message.setGlyphColor({0.82f, 0.87f, 0.94f, 1.0f});
			_message.setOutlineColor({0.03f, 0.05f, 0.08f, 1.0f});
			_message.setAlignment({spk::Alignment::Horizontal::Center, spk::Alignment::Vertical::Center});
			setMessage("Workspace content begins below its MenuBar. The ComboBox reuses its TextModel delegate for the collapsed value and opens its DataModel view in the popup render pass.");
			_comboContract = _combo.subscribeToSelection([this](std::optional<spk::ComboBox::Selection> selection) {
				setMessage(selection.has_value() ? "ComboBox selected: " + _model.data(selection->row) : "ComboBox selection cleared.");
			});
			_listContract = _list.subscribeToSelection([this](std::optional<spk::TextModel::View::Selection> selection) {
				setMessage(selection.has_value() ? "TextModel view selected row " + std::to_string(selection->row) + ": " + _model.data(selection->row) : "TextModel view selection cleared.");
			});
		}

		void setMessage(std::string_view message)
		{
			_message.setText(message);
		}
	};

	class MenusShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::Workspace<MenuWorkspaceContent> _workspace;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			const unsigned int height = _surface.geometry().height;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_workspace.setGeometry(rectangle(24, 70, reduced(width, 48), reduced(height, 94)));
		}

		void _configureMenuButton(spk::PushButton &button)
		{
			button.setFlat(true);
		}

		void _configurePopup(spk::MenuBar::Menu &menu)
		{
			menu.background().setCornerSize({10, 10});
		}

	public:
		MenusShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_workspace(this->name() + ".workspace", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Popup menus, model-backed selectors, and typed workspaces", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_workspace.content().configure();
			spk::MenuBar &menuBar = _workspace.menuBar();
			menuBar.background().setCornerSize({6, 6});
			menuBar.setHeight(34);

			spk::MenuBar::Menu &file = menuBar.addMenu("file", "File");
			_configureMenuButton(menuBar.button("file"));
			_configurePopup(file);
			_configureMenuButton(file.addItem("new", "New", [this]() {
				_workspace.content().setMessage("File > New selected. The popup closed itself after invoking this callback.");
			}));
			_configureMenuButton(file.addItem("open", "Open", [this]() {
				_workspace.content().setMessage("File > Open selected.");
			}));
			file.addBreak("primary");
			_configureMenuButton(file.addItem("quit", "Quit showcase", [this]() {
				_workspace.content().setMessage("The showcase keeps running; this item only demonstrates a menu action.");
			}));

			spk::MenuBar::Menu &edit = menuBar.addMenu("edit", "Edit");
			_configureMenuButton(menuBar.button("edit"));
			_configurePopup(edit);
			_configureMenuButton(edit.addItem("copy", "Copy", [this]() {
				_workspace.content().setMessage("Edit > Copy selected.");
			}));
			_configureMenuButton(edit.addItem("paste", "Paste", [this]() {
				_workspace.content().setMessage("Edit > Paste selected.");
			}));

			spk::MenuBar::Menu &empty = menuBar.addMenu("empty", "Empty");
			_configureMenuButton(menuBar.button("empty"));
			_configurePopup(empty);
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class DialogsShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::PushButton _reopenButton;
		spk::InformationMessageBox _information;
		spk::RequestMessageBox _request;
		spk::TextLabel _status;
		spk::PushButton::ClickContract _reopenContract;
		spk::PushButton::ClickContract _informationContract;

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			const unsigned int halfWidth = reduced(width, 78) / 2;
			_title.setGeometry(rectangle(24, 18, reduced(width, 270), 38));
			_reopenButton.setGeometry(rectangle(static_cast<int>(reduced(width, 222)), 14, 198, 42));
			_information.setGeometry(rectangle(24, 82, halfWidth, 360));
			_request.setGeometry(rectangle(static_cast<int>(54 + halfWidth), 82, halfWidth, 360));
			_status.setGeometry(rectangle(30, 470, reduced(width, 60), 38));
		}

		void _configureMessage(spk::MessageBox &messageBox)
		{
			messageBox.textArea().setTextSize(17);
			messageBox.textArea().setLinePadding(5);
			messageBox.textArea().setGlyphColor({0.92f, 0.95f, 1.0f, 1.0f});
			messageBox.textArea().setOutlineColor({0.03f, 0.04f, 0.08f, 1.0f});
			messageBox.setMinimalWidth(250);
		}

	public:
		DialogsShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_reopenButton(this->name() + ".reopen", &_surface),
			_information(this->name() + ".information", &_surface),
			_request(this->name() + ".request", &_surface),
			_status(this->name() + ".status", &_surface)
		{
			_surface.setCornerSize({12, 12});
			configureText(_title, "Information and request message boxes", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_reopenButton.setText("Reopen dialogs");
			_request.setMenuButtonEnabled(spk::IInterfaceWindow::MenuBar::Button::Minimize, false);
			_configureMessage(_information);
			_configureMessage(_request);
			_information.setText("InformationMessageBox provides one Close command. Its title-bar close control uses the same generic close behavior.");
			_request.setText("RequestMessageBox associates the title-bar close control with its second, cancel-like action.");
			configureText(_status, "Choose a request action or close either message box.", 16);
			_status.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_reopenContract = _reopenButton.subscribeToClick([this]() {
				_information.activate();
				_request.activate();
				_status.setText("Both message boxes reopened.");
			});
			_informationContract = _information.subscribe("close", [this]() {
				_status.setText("Information message closed.");
			});
			_request.configure(
				"Accept",
				[this]() {
					_status.setText("Request accepted.");
				},
				"Cancel",
				[this]() {
					_status.setText("Request cancelled.");
				});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class DebugShowcasePage final : public spk::Widget
	{
		bool _layoutReady = false;
		spk::Panel _surface;
		spk::TextLabel _title;
		spk::Panel _tableBackground;
		spk::DebugOverlay _overlay;
		std::vector<std::unique_ptr<spk::TextLabel>> _labels;
		spk::PushButton _customCell;
		spk::TextLabel _caption;
		spk::PushButton::ClickContract _customCellContract;

		void _addText(std::size_t row, std::size_t column, std::string_view text)
		{
			auto label = std::make_unique<spk::TextLabel>(
				name() + ".label-" + std::to_string(row) + "-" + std::to_string(column), &_overlay);
			configureText(*label, text, 18);
			label->setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			label->setMaximalSize({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()});
			_overlay.setWidget(column, row, label.get());
			_labels.push_back(std::move(label));
		}

		void _onGeometryChange() override
		{
			if (!_layoutReady)
			{
				return;
			}
			_surface.setGeometry(rectangle(12, 12, reduced(geometry().width, 24), reduced(geometry().height, 24)));
			const unsigned int width = _surface.geometry().width;
			const unsigned int height = _surface.geometry().height;
			_title.setGeometry(rectangle(24, 18, reduced(width, 48), 38));
			_tableBackground.setGeometry(rectangle(24, 76, reduced(width, 48), reduced(height, 154)));
			_overlay.setGeometry(rectangle(
				14, 14, reduced(_tableBackground.geometry().width, 28), reduced(_tableBackground.geometry().height, 28)));
			_caption.setGeometry(rectangle(28, static_cast<int>(reduced(height, 62)), reduced(width, 56), 34));
		}

	public:
		DebugShowcasePage(std::string name, spk::Widget *parent) :
			Widget(std::move(name), parent),
			_surface(this->name() + ".surface", this),
			_title(this->name() + ".title", &_surface),
			_tableBackground(this->name() + ".table-background", &_surface),
			_overlay(this->name() + ".overlay", &_tableBackground),
			_customCell(this->name() + ".custom-cell", &_overlay),
			_caption(this->name() + ".caption", &_surface)
		{
			_surface.setCornerSize({12, 12});
			_tableBackground.setCornerSize({14, 14});
			configureText(_title, "DebugOverlay - widget grid host", 28);
			_title.setHorizontalAlignment(spk::Alignment::Horizontal::Left);

			_overlay.setElementPadding({12, 10});
			_addText(0, 0, "Frame");
			_addText(0, 1, "16.67 ms");
			_addText(0, 2, "Stable");
			_addText(1, 0, "Renderer");
			_addText(1, 1, "OpenGL");
			_addText(1, 2, "1920 x 1080");
			_addText(1, 3, "Present");
			_addText(2, 0, "Keyboard focus");
			_addText(2, 1, "TextEdit");
			_addText(2, 2, "Owned by page");

			_customCell.setText("Custom PushButton cell");
			_customCell.setMaximalSize({std::numeric_limits<float>::max(), std::numeric_limits<float>::max()});
			_overlay.setWidget(0, 3, &_customCell);
			_addText(4, 3, "Auto-expanded cell [4,3]");

			configureText(_caption, "The page owns every cell; DebugOverlay only arranges its child widgets.", 16);
			_caption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_customCellContract = _customCell.subscribeToClick([this]() {
				_caption.setText("The custom PushButton received the click from inside the grid.");
			});
			_layoutReady = true;
			_onGeometryChange();
			activate();
		}
	};

	class WidgetShowcase
	{
		spk::Screen _screen;
		spk::Panel _background;
		spk::Panel _sidebar;
		spk::Panel _contentSurface;
		spk::TextLabel _brand;
		spk::TextLabel _navigationCaption;
		spk::TextLabel _pageTitle;
		spk::TextLabel _subtitle;
		spk::TabWidget _tabs;
		spk::IScrollArea _textScroll;
		spk::IScrollArea _interactionScroll;
		spk::IScrollArea _scrollingScroll;
		spk::IScrollArea _windowsScroll;
		spk::IScrollArea _panelsScroll;
		spk::IScrollArea _menusScroll;
		spk::IScrollArea _dialogsScroll;
		spk::IScrollArea _debugScroll;
		TextShowcasePage _textPage;
		InteractionShowcasePage _interactionPage;
		ScrollingShowcasePage _scrollingPage;
		WindowsShowcasePage _windowsPage;
		PanelsShowcasePage _panelsPage;
		MenusShowcasePage _menusPage;
		DialogsShowcasePage _dialogsPage;
		DebugShowcasePage _debugPage;
		std::vector<std::unique_ptr<spk::PushButton>> _navigationButtons;
		std::vector<spk::PushButton::ClickContract> _navigationContracts;

		void _selectPage(std::size_t index, std::string_view title)
		{
			_tabs.selectPage(index);
			_pageTitle.setText(title);
			for (std::size_t buttonIndex = 0; buttonIndex < _navigationButtons.size(); ++buttonIndex)
			{
				_navigationButtons[buttonIndex]->setFlat(buttonIndex != index);
			}
		}

		void _addNavigationButton(std::string_view label, std::string_view title, std::size_t index)
		{
			auto button = std::make_unique<spk::PushButton>("showcase.navigation." + std::to_string(index), &_sidebar);
			button->releasedBackground().setSpriteSheet(spk::Widget::defaultStyle->lightNineSlice.get());
			button->setText(label);
			button->setAlignment({spk::Alignment::Horizontal::Left, spk::Alignment::Vertical::Center});
			button->setTextPadding({18, 10});
			button->setFlat(index != 0);
			button->setGeometry(rectangle(14, static_cast<int>(138 + index * 54), 202, 44));
			_navigationContracts.push_back(button->subscribeToClick([this, index, title = std::string(title)]() {
				_selectPage(index, title);
			}));
			_navigationButtons.push_back(std::move(button));
		}

		void _configurePageScroll(spk::IScrollArea &scrollArea, spk::Widget &page)
		{
			scrollArea.setScrollBarWidth(20);
			page.setMinimalSize({760.0f, 640.0f});
			scrollArea.setContent(&page);
			scrollArea.verticalScrollBar().negativeButton().setIconSize({10, 10});
			scrollArea.verticalScrollBar().negativeButton().setIconPadding({4, 4});
			scrollArea.verticalScrollBar().positiveButton().setIconSize({10, 10});
			scrollArea.verticalScrollBar().positiveButton().setIconPadding({4, 4});
		}

	public:
		explicit WidgetShowcase(spk::Widget &root) :
			_screen("showcase.screen", &root),
			_background("showcase.background", &_screen),
			_sidebar("showcase.sidebar", &_background),
			_contentSurface("showcase.content-surface", &_background),
			_brand("showcase.brand", &_sidebar),
			_navigationCaption("showcase.navigation-caption", &_sidebar),
			_pageTitle("showcase.page-title", &_contentSurface),
			_subtitle("showcase.subtitle", &_contentSurface),
			_tabs("showcase.tabs", &_contentSurface),
			_textScroll("showcase.scroll.text", &_tabs),
			_interactionScroll("showcase.scroll.interaction", &_tabs),
			_scrollingScroll("showcase.scroll.scrolling", &_tabs),
			_windowsScroll("showcase.scroll.windows", &_tabs),
			_panelsScroll("showcase.scroll.panels", &_tabs),
			_menusScroll("showcase.scroll.menus", &_tabs),
			_dialogsScroll("showcase.scroll.dialogs", &_tabs),
			_debugScroll("showcase.scroll.debug", &_tabs),
			_textPage("showcase.page.text", &_textScroll.container()),
			_interactionPage("showcase.page.interaction", &_interactionScroll.container()),
			_scrollingPage("showcase.page.scrolling", &_scrollingScroll.container()),
			_windowsPage("showcase.page.windows", &_windowsScroll.container()),
			_panelsPage("showcase.page.panels", &_panelsScroll.container()),
			_menusPage("showcase.page.menus", &_menusScroll.container()),
			_dialogsPage("showcase.page.dialogs", &_dialogsScroll.container()),
			_debugPage("showcase.page.debug", &_debugScroll.container())
		{
			_screen.setGeometry(rectangle(0, 0, WindowSize.x, WindowSize.y));
			_background.setGeometry(rectangle(0, 0, WindowSize.x, WindowSize.y));
			_background.setCornerSize({0, 0});
			_sidebar.setGeometry(rectangle(0, 0, 230, WindowSize.y));
			_sidebar.setCornerSize({0, 0});
			_sidebar.setSpriteSheet(spk::Widget::defaultStyle->darkerNineSlice.get());
			_contentSurface.setGeometry(rectangle(230, 0, WindowSize.x - 230, WindowSize.y));
			_contentSurface.setCornerSize({0, 0});

			configureText(_brand, "Sparkle UI", 30);
			_brand.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_brand.setGeometry(rectangle(22, 20, 190, 42));
			configureText(_navigationCaption, "COMPONENTS", 13);
			_navigationCaption.setGlyphColor({0.48f, 0.58f, 0.72f, 1.0f});
			_navigationCaption.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_navigationCaption.setGeometry(rectangle(22, 100, 190, 24));

			configureText(_pageTitle, "Typography and text input", 32);
			_pageTitle.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_pageTitle.setGeometry(rectangle(30, 22, 810, 46));
			configureText(_subtitle, "A complete tour of Sparkle's native widgets and interaction states", 16);
			_subtitle.setGlyphColor({0.64f, 0.72f, 0.84f, 1.0f});
			_subtitle.setHorizontalAlignment(spk::Alignment::Horizontal::Left);
			_subtitle.setGeometry(rectangle(32, 72, 808, 28));
			_tabs.setTabBarHeight(0);
			_tabs.setTabWidth(0);
			_tabs.setContentTopPadding(0);
			_tabs.setGeometry(rectangle(28, 116, 814, 616));
			_configurePageScroll(_textScroll, _textPage);
			_configurePageScroll(_interactionScroll, _interactionPage);
			_configurePageScroll(_scrollingScroll, _scrollingPage);
			_configurePageScroll(_windowsScroll, _windowsPage);
			_configurePageScroll(_panelsScroll, _panelsPage);
			_configurePageScroll(_menusScroll, _menusPage);
			_configurePageScroll(_dialogsScroll, _dialogsPage);
			_configurePageScroll(_debugScroll, _debugPage);
			_tabs.addPage("Text", &_textScroll);
			_tabs.addPage("Interaction", &_interactionScroll);
			_tabs.addPage("Scrolling", &_scrollingScroll);
			_tabs.addPage("Windows", &_windowsScroll);
			_tabs.addPage("Panels", &_panelsScroll);
			_tabs.addPage("Menus", &_menusScroll);
			_tabs.addPage("Dialogs", &_dialogsScroll);
			_tabs.addPage("Debug", &_debugScroll);

			_addNavigationButton("Typography", "Typography and text input", 0);
			_addNavigationButton("Controls", "Controls and interaction", 1);
			_addNavigationButton("Scrolling", "Scrolling and progress", 2);
			_addNavigationButton("Windows", "Window composition", 3);
			_addNavigationButton("Panels", "Panels and prompts", 4);
			_addNavigationButton("Menus", "Menus and model-backed selectors", 5);
			_addNavigationButton("Dialogs", "Dialogs and decisions", 6);
			_addNavigationButton("Debug", "Debug layout tools", 7);
			_selectPage(0, "Typography and text input");
			_screen.activate();
		}
	};
}

int main()
{
	spk::Application application;
	spk::Window &window = application.createWindow(
		"sparkle-widget-showcase",
		spk::Window::Configuration{
			.title = "Sparkle UI showcase",
			.area = rectangle(120, 80, WindowSize.x, WindowSize.y),
			.backgroundColor = {0.018f, 0.024f, 0.045f, 1.0f}});
	window.root().setGeometry(rectangle(0, 0, WindowSize.x, WindowSize.y));
	WidgetShowcase showcase(window.root());
	return application.run();
}

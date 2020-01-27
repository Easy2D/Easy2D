#pragma once
#include <easy2d/e2dbase.h>

namespace easy2d 
{


class Action;
class Transition;
class SceneManager;

class Node :
	public Object
{
	friend class Scene;
	friend class Transition;
	friend class SceneManager;

public:
	// 节点属性
	struct Property
	{
		bool visable;		// 可见性
		float posX;			// X 坐标
		float posY;			// Y 坐标
		float width;		// 宽度
		float height;		// 高度
		float opacity;		// 透明度
		float anchorX;		// 锚点 X 坐标
		float anchorY;		// 锚点 Y 坐标
		float scaleX;		// 横向缩放
		float scaleY;		// 纵向缩放
		float rotation;		// 旋转角度
		float skewAngleX;	// 横向倾斜角度
		float skewAngleY;	// 纵向倾斜角度
	};

public:
	Node();

	virtual ~Node();

	// 更新节点
	virtual void onUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 获取节点显示状态
	bool isVisible() const;

	// 获取节点名称
	String getName() const;

	// 获取节点名称的 Hash 值
	size_t getHashName() const;

	// 获取节点绘图顺序
	int getOrder() const;

	// 获取节点横坐标
	float getPosX() const;

	// 获取节点纵坐标
	float getPosY() const;

	// 获取节点坐标
	Point getPos() const;

	// 获取节点宽度
	float getWidth() const;

	// 获取节点高度
	float getHeight() const;

	// 获取节点宽度（不考虑缩放）
	float getRealWidth() const;

	// 获取节点高度（不考虑缩放）
	float getRealHeight() const;

	// 获取节点大小（不考虑缩放）
	Size getRealSize() const;

	// 获取节点的锚点
	float getAnchorX() const;

	// 获取节点的锚点
	float getAnchorY() const;

	// 获取节点大小
	Size getSize() const;

	// 获取节点横向缩放比例
	float getScaleX() const;

	// 获取节点纵向缩放比例
	float getScaleY() const;

	// 获取节点横向倾斜角度
	float getSkewX() const;

	// 获取节点纵向倾斜角度
	float getSkewY() const;

	// 获取节点旋转角度
	float getRotation() const;

	// 获取节点透明度
	float getOpacity() const;

	// 获取节点属性
	Property getProperty() const;

	// 获取边框
	Rect getBounds() const;

	// 获取外切包围盒
	Rect getBoundingBox() const;

	// 获取二维变换矩阵
	Matrix32 getTransform() const;

	// 获取二维变换逆矩阵
	Matrix32 getInverseTransform() const;

	// 获取父节点
	Node * getParent() const;

	// 获取节点所在场景
	Scene * getParentScene() const;

	// 是否包含点坐标
	bool containsPoint(Point const& point);

	// 设置节点是否显示
	void setVisible(
		bool value
	);

	// 开启或禁用 onUpdate 函数
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// 设置节点名称
	void setName(
		const String& name
	);

	// 设置节点横坐标
	void setPosX(
		float x
	);

	// 设置节点纵坐标
	void setPosY(
		float y
	);

	// 设置节点坐标
	void setPos(
		const Point & point
	);

	// 设置节点坐标
	void setPos(
		float x,
		float y
	);

	// 节点坐标固定
	void setPosFixed(
		bool fixed
	);

	// 移动节点
	void movePosX(
		float x
	);

	// 移动节点
	void movePosY(
		float y
	);

	// 移动节点
	void movePos(
		float x,
		float y
	);

	// 移动节点
	void movePos(
		const Vector2 & v
	);

	// 设置节点绘图顺序
	// 默认为 0
	void setOrder(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0f
	void setScaleX(
		float scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0f
	void setScaleY(
		float scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0f, 1.0f)
	void setScale(
		float scaleX,
		float scaleY
	);

	// 设置缩放比例
	// 默认为 1.0f
	void setScale(
		float scale
	);

	// 设置横向倾斜角度
	// 默认为 0
	void setSkewX(
		float angleX
	);

	// 设置纵向倾斜角度
	// 默认为 0
	void setSkewY(
		float angleY
	);

	// 设置倾斜角度
	// 默认为 (0, 0)
	void setSkew(
		float angleX,
		float angleY
	);

	// 设置旋转角度
	// 默认为 0
	void setRotation(
		float rotation
	);

	// 设置透明度
	// 默认为 1.0f, 范围 [0, 1]
	void setOpacity(
		float opacity
	);

	// 设置锚点的横向位置
	// 默认为 0, 范围 [0, 1]
	void setAnchorX(
		float anchorX
	);

	// 设置锚点的纵向位置
	// 默认为 0, 范围 [0, 1]
	void setAnchorY(
		float anchorY
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	void setAnchor(
		float anchorX,
		float anchorY
	);

	// 修改节点宽度
	void setWidth(
		float width
	);

	// 修改节点高度
	void setHeight(
		float height
	);

	// 修改节点大小
	void setSize(
		float width,
		float height
	);

	// 修改节点大小
	void setSize(
		Size size
	);

	// 设置节点属性
	void setProperty(
		Property prop
	);

	// 添加子节点
	void addChild(
		Node * child,
		int order = 0	/* 渲染顺序 */
	);

	// 添加多个子节点
	void addChild(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 获取所有名称相同的子节点
	std::vector<Node*> getChildren(
		const String& name
	) const;

	// 获取名称相同的子节点
	Node* getChild(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAllChildren() const;

	// 获取子节点数量
	int getChildrenCount() const;

	// 移除子节点
	bool removeChild(
		Node* child
	);

	// 移除所有名称相同的子节点
	void removeChildren(
		const String& childName
	);

	// 从父节点移除
	void removeFromParent();

	// 移除所有节点
	void removeAllChildren();

	// 分发事件
	void dispatch(Event* evt);

	// 执行动作
	void runAction(
		Action * action
	);

	// 继续动作
	void resumeAction(
		const String& name
	);

	// 暂停动作
	void pauseAction(
		const String& name
	);

	// 停止动作
	void stopAction(
		const String& name
	);

	// 继续所有暂停动作
	void resumeAllActions();

	// 暂停所有动作
	void pauseAllActions();

	// 停止所有动作
	void stopAllActions();

	// 添加输入监听
	Listener* addListener(
		const Listener::Callback& func,	/* 监听到用户输入时的执行函数 */
		const String& name = L"",		/* 监听器名称 */
		bool paused = false				/* 是否暂停 */
	);

	// 添加碰撞监听
	void addListener(
		Listener* listener		/* 监听器 */
	);

	// 移除监听器
	void removeListener(
		Listener* listener		/* 监听器 */
	);

	// 启动输入监听
	void startListener(
		const String& name
	);

	// 停止输入监听
	void stopListener(
		const String& name
	);

	// 移除输入监听
	void removeListener(
		const String& name
	);

	// 启动所有监听器
	void startAllListeners();

	// 停止所有监听器
	void stopAllListeners();

	// 移除所有监听器
	void removeAllListeners();

	// 修改节点的默认锚点位置
	static void setDefaultAnchor(
		float defaultAnchorX,
		float defaultAnchorY
	);

protected:
	// 更新节点
	void _update();

	// 渲染节点
	void _render();

	// 设置节点所在场景
	void _setParentScene(
		Scene * scene
	);

	// 更新二维变换矩阵
	void _updateTransform() const;

	// 更新二维变换逆矩阵
	void _updateInverseTransform() const;

	// 子节点排序
	void _sortChildren();

	// 更新节点透明度
	void _updateOpacity();

	// 更新监听器
	void __updateListeners(Event* evt);

	// 清空监听器
	void __clearListeners();

protected:
	bool		_visible;
	bool		_autoUpdate;
	bool		_needSort;
	bool		_positionFixed;
	float		_posX;
	float		_posY;
	float		_width;
	float		_height;
	float		_scaleX;
	float		_scaleY;
	float		_rotation;
	float		_skewAngleX;
	float		_skewAngleY;
	float		_displayOpacity;
	float		_realOpacity;
	float		_anchorX;
	float		_anchorY;
	int			_nOrder;
	String		_name;
	size_t		_hashName;
	Scene *		_parentScene;
	Node *		_parent;
	
	std::vector<Node*>	_children;
	std::vector<Listener*> _listeners;

	mutable bool		_dirtyTransform;
	mutable Matrix32	_transform;
	mutable bool		_dirtyInverseTransform;
	mutable Matrix32	_inverseTransform;
};


// 场景
class Scene :
	public Node
{
public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在关闭窗口时执行（返回 false 将阻止窗口关闭）
	virtual bool onCloseWindow() { return true; }
};


// 精灵
class Sprite :
	public Node
{
public:
	Sprite();

	explicit Sprite(
		Image * image
	);

	explicit Sprite(
		const String& filePath	/* 图片文件路径 */
	);

	explicit Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	explicit Sprite(
		const String& filePath,	/* 图片文件路径 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	explicit Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	virtual ~Sprite();

	// 加载图片文件
	bool open(
		const String& filePath
	);

	// 加载图片资源
	bool open(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);
	
	// 加载图片
	bool open(
		Image * image
	);

	// 将图片裁剪为矩形
	void crop(
		const Rect& cropRect	/* 裁剪矩形 */
	);

	// 获取 Image 对象
	virtual Image * getImage() const;

	// 渲染精灵
	virtual void onRender() override;

protected:
	Image * _image;
};


// 文本
class Text :
	public Node
{
public:
	// 文本对齐方式
	enum class Align
	{
		Left,		/* 左对齐 */
		Right,		/* 右对齐 */
		Center		/* 居中对齐 */
	};

	// 文本样式
	class Style
	{
	public:
		Color		color;				// 颜色
		Align		alignment;			// 对齐方式
		bool		wrapping;			// 打开自动换行
		float		wrappingWidth;		// 自动换行宽度
		float		lineSpacing;		// 行间距
		bool		hasUnderline;		// 下划线
		bool		hasStrikethrough;	// 删除线
		bool		hasOutline;			// 显示描边
		Color		outlineColor;		// 描边颜色
		float		outlineWidth;		// 描边线宽
		LineJoin	outlineJoin;		// 描边线相交样式

	public:
		Style();

		Style(
			Color color,
			Align alignment = Align::Left,
			bool wrapping = false,
			float wrappingWidth = 0.0,
			float lineSpacing = 0.0,
			bool hasUnderline = false,
			bool hasStrikethrough = false,
			bool hasOutline = true,
			Color outlineColor = Color(Color::Black, 0.5),
			float outlineWidth = 1.0,
			LineJoin outlineJoin = LineJoin::Round
		);
	};

public:
	Text();

	explicit Text(
		const String& text,						/* 文字内容 */
		const Font& font = Font(),				/* 字体 */
		const Style& style = Style()			/* 文本样式 */
	);

	virtual ~Text();

	// 获取文本
	String getText() const;

	// 获取字体
	Font getFont() const;

	// 获取文本样式
	Style getStyle() const;

	// 获取字体族
	String getFontFamily() const;

	// 获取当前字号
	float getFontSize() const;

	// 获取当前字体粗细值
	UINT getFontWeight() const;

	// 获取文字颜色
	Color getColor() const;

	// 获取描边颜色
	Color getOutlineColor() const;

	// 获取描边线宽
	float getOutlineWidth() const;

	// 获取描边线相交样式
	LineJoin getOutlineJoin() const;

	// 获取文本显示行数
	int getLineCount() const;

	// 是否是斜体
	bool isItalic() const;

	// 是否显示删除线
	bool hasStrikethrough() const;

	// 是否显示下划线
	bool hasUnderline() const;

	// 是否显示描边
	bool hasOutline() const;

	// 设置文本
	void setText(
		const String& text
	);

	// 设置文本样式
	void setStyle(
		const Style& style
	);

	// 设置字体
	void setFont(
		const Font& font
	);

	// 设置字体族
	void setFontFamily(
		const String& family
	);

	// 设置字号（默认值为 22）
	void setFontSize(
		float size
	);

	// 设置字体粗细值（默认值为 Text::Font::Weight::Normal）
	void setFontWeight(
		UINT weight
	);

	// 设置文字颜色（默认值为 Color::WHITE）
	void setColor(
		Color color
	);

	// 设置文字斜体（默认值为 false）
	void setItalic(
		bool value
	);

	// 打开或关闭文本自动换行（默认为关闭）
	void setWrapping(
		bool wrapping
	);

	// 设置文本自动换行的宽度（默认为 0）
	void setWrappingWidth(
		float wrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		float lineSpacing
	);

	// 设置对齐方式（默认为 Align::Left）
	void setAlignment(
		Align align
	);

	// 设置下划线（默认值为 false）
	void setUnderline(
		bool hasUnderline
	);

	// 设置删除线（默认值为 false）
	void setStrikethrough(
		bool hasStrikethrough
	);

	// 设置是否显示描边
	void setOutline(
		bool hasOutline
	);

	// 设置描边颜色
	void setOutlineColor(
		Color outlineColor
	);

	// 设置描边线宽
	void setOutlineWidth(
		float outlineWidth
	);

	// 设置描边线相交样式
	void setOutlineJoin(
		LineJoin outlineJoin
	);

	// 渲染文字
	virtual void onRender() override;

protected:
	// 重新排版文字
	void _reset();

	// 创建文字格式化
	void _createFormat();

	// 创建文字布局
	void _createLayout();

protected:
	String	_text;
	Font	_font;
	Style	_style;
	IDWriteTextFormat * _textFormat;
	IDWriteTextLayout * _textLayout;
};


class Button :
	public Node
{
public:
	using Callback = Function<void()>;

	Button();

	explicit Button(
		Node * normal,					/* 普通状态 */
		const Callback& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
		Node * normal,					/* 普通状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Callback& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
		Node * normal,					/* 普通状态 */
		Node * mouseover,				/* 鼠标移入状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Callback& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
		Node * normal,					/* 普通状态 */
		Node * mouseover,				/* 鼠标移入状态 */
		Node * selected,				/* 鼠标移入状态 */
		Node * disabled,				/* 按钮禁用状态 */
		const Callback& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 获取按钮状态是启用还是禁用
	bool isEnable() const;

	// 设置按钮启用或禁用
	void setEnable(
		bool enable
	);

	// 设置一般情况下显示的按钮
	virtual void setNormal(
		Node * normal
	);

	// 设置鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		Node * mouseover
	);

	// 设置鼠标按下按钮时显示的按钮
	virtual void setSelected(
		Node * selected
	);

	// 设置按钮被禁用时显示的按钮
	virtual void setDisabled(
		Node * disabled
	);

	// 设置按钮点击后的执行函数
	void setClickFunc(
		const Callback& func
	);

protected:
	// 按钮状态枚举
	enum class ButtonState { Normal, Mouseover, Selected };

	// 设置按钮状态
	virtual void _setState(ButtonState state);

	// 刷新按钮显示
	virtual void _updateVisiable();

	// 执行按钮函数对象
	virtual void _runCallback();

	void updateStatus(Event* evt);

protected:
	bool		_enable;
	bool		_isSelected;
	bool		_isHover;
	bool		_isPressed;
	Node *		_normal;
	Node *		_mouseover;
	Node *		_selected;
	Node *		_disabled;
	ButtonState	_state;
	Callback	_func;
};


class ToggleButton :
	public Button
{
public:
	ToggleButton();

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		const Function<void()>& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function<void()>& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onMouseOver,				/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,			/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function<void()>& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onMouseOver,				/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,			/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		Node * onDisabled,				/* 按钮打开时，禁用状态 */
		Node * offDisabled,				/* 按钮关闭时，禁用状态 */
		const Function<void()>& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 获取开关状态（打开或关闭）
	bool getState() const;

	// 设置开关按钮的状态（打开或关闭）
	void setState(
		bool bState
	);

	// 设置按钮打开状态下显示的按钮
	virtual void setNormal(
		Node * normal
	) override;

	// 设置按钮打开状态下，鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		Node * mouseover
	) override;

	// 设置按钮打开状态下，鼠标按下按钮时显示的按钮
	virtual void setSelected(
		Node * selected
	) override;

	// 设置按钮打开状态下，被禁用时显示的按钮
	virtual void setDisabled(
		Node * disabled
	) override;

	// 设置按钮关闭状态下显示的按钮
	void setNormalOff(
		Node * normal
	);

	// 设置按钮关闭状态下，鼠标移入按钮时显示的按钮
	void setMouseOverOff(
		Node * mouseover
	);

	// 设置按钮关闭状态下，鼠标按下按钮时显示的按钮
	void setSelectedOff(
		Node * selected
	);

	// 设置按钮关闭状态下，按钮被禁用时显示的按钮
	void setDisabledOff(
		Node * disabled
	);

protected:
	// 刷新按钮开关
	virtual void _updateState();

	// 执行按钮函数对象
	virtual void _runCallback() override;

protected:
	Node *	_normalOff;
	Node *	_mouseoverOff;
	Node *	_selectedOff;
	Node *	_disabledOff;
	bool	_toggle;
};


class Menu :
	public Node
{
public:
	Menu();

	explicit Menu(
		const std::vector<Button*>& buttons	/* 按钮数组 */
	);

	// 获取菜单是否禁用
	bool isEnable() const;

	// 获取菜单中的按钮数量
	size_t getButtonCount() const;

	// 设置菜单启用或禁用
	void setEnable(
		bool enable
	);

	// 添加按钮
	void addButton(
		Button * button
	);

	// 移除按钮
	bool removeButton(
		Button * button
	);

	// 获取所有按钮
	const std::vector<Button*>& getAllButtons() const;

protected:
	bool _enable;
	std::vector<Button*> _buttons;
};

}
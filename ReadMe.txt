"$(COCOS2D_20_RC2_x_201)CocosDenshion\include";..\Classes;$(COCOS2D_20_RC2_x_201)cocos2dx;$(COCOS2D_20_RC2_x_201)cocos2dx\platform;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\win32;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\iconv;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\zlib;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libpng;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libjpeg;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libtiff;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libxml2;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\pthread;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\OGLES;$(COCOS2D_20_RC2_x_201)cocos2dx\include;$(COCOS2D_20_RC2_x_201)cocos2dx\kazmath\include;$(COCOS2D_20_RC2_x_201)cocos2dx\actions;$(COCOS2D_20_RC2_x_201)cocos2dx\base_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\cocoa;$(COCOS2D_20_RC2_x_201)cocos2dx\effects;$(COCOS2D_20_RC2_x_201)cocos2dx\extensions;$(COCOS2D_20_RC2_x_201)cocos2dx\keypad_dispatcher;$(COCOS2D_20_RC2_x_201)cocos2dx\label_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\layers_scenes_transitions_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\menu_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\misc_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\particle_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\script_support;$(COCOS2D_20_RC2_x_201)cocos2dx\shaders;$(COCOS2D_20_RC2_x_201)cocos2dx\sprite_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\text_input_node;$(COCOS2D_20_RC2_x_201)cocos2dx\textures;$(COCOS2D_20_RC2_x_201)cocos2dx\tileMap_parallax_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\touch_dispatcher;$(COCOS2D_20_RC2_x_201)cocos2dx\support



第一阶段，只要实现一条鱼动画即可。动画约定在8帧之内。玩家视点在鱼的左侧。

因为是辅助角色，所以动画不用过于详细，动画可以比较简单。


AxisBoundingBox

以下的设定可以在第二版中出现。先期可以是Human vs Human。

在遥远星际，有一个种族（the Wraith）（幽灵族，星际之门中的），他们统治着很多星球。在那里远行已经可以通过星际之门实现，各个星球对于行旅者就如同一个岛屿（Island）一样。

人类在拓展自己的活动范围，有一天，我们接触到了the Wraith, 为了争夺资源，我们建立了基地，开始了和他们的战争。

Wraith特点：

＊不伤害人类，只是吸食生命，吸食了生命会使他们变得特别强大。

＊人数不多，因为他们严格限制自己的数量，否则他们的掠食资源不够多，会导致他们种族内战，他们可以吸食自己。当需要捍卫生存权利的时候，他们会自己吃自己。

＊ 种类：

母后：医疗兵，生活在母巢中，可以远距离加血，最多同时出现三个。前线兵种吸食的生命可以汇集到她身上，然后由她分配。

战士：近战

击晕兵：击晕枪，将人击晕5秒，然后吸食。吸食后变强（？）。

资源收集者：Dart，一种小飞机，飞临水晶资源区，停留5秒进行采集，然后飞回去，卸货。最多同时制造5个。

人类特点:

面对Wraith，就是杀无赦。狂轰滥炸，无任何道义而言。

人数众多，但是质量不敌Wraith，血量少。

种类：

医疗兵：MM，可爱的护士，但是需要上前线，人类的后卫。

战士：近战，重甲，攻击低，会武术，谁也挡不住。

枪兵：远程，机关枪。轻甲，伤害高。

采矿机甲：小悬浮机器人，采矿。

游戏中的两方设定就是 Wraith vs Human。

你可以选择其中一方，永远不可以改变。

双方都有自己的科技树。

资源：水晶，这是一切星际能源的基础。

大舰（人类），母巢（Wraith）：生产基地，利用水晶提供的能源，制造各种兵种。

科技树，采用魔兽世界的天赋点方式。每关的胜利，可以获得1-3个天赋点，你不想打三星
没有关系，后面你真过不去。

Wraith：



to 2.0
"$(COCOS2D_20_RC2_x_201)CocosDenshion\include";..\Classes;$(COCOS2D_20_RC2_x_201)cocos2dx;$(COCOS2D_20_RC2_x_201)cocos2dx\platform;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\win32;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\iconv;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\zlib;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libpng;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libjpeg;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libtiff;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libxml2;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\pthread;$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\OGLES;$(COCOS2D_20_RC2_x_201)cocos2dx\include;$(COCOS2D_20_RC2_x_201)cocos2dx\kazmath\include;$(COCOS2D_20_RC2_x_201)cocos2dx\actions;$(COCOS2D_20_RC2_x_201)cocos2dx\base_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\cocoa;$(COCOS2D_20_RC2_x_201)cocos2dx\effects;$(COCOS2D_20_RC2_x_201)cocos2dx\extensions;$(COCOS2D_20_RC2_x_201)cocos2dx\keypad_dispatcher;$(COCOS2D_20_RC2_x_201)cocos2dx\label_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\layers_scenes_transitions_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\menu_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\misc_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\particle_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\script_support;$(COCOS2D_20_RC2_x_201)cocos2dx\shaders;$(COCOS2D_20_RC2_x_201)cocos2dx\sprite_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\text_input_node;$(COCOS2D_20_RC2_x_201)cocos2dx\textures;$(COCOS2D_20_RC2_x_201)cocos2dx\tileMap_parallax_nodes;$(COCOS2D_20_RC2_x_201)cocos2dx\touch_dispatcher;$(COCOS2D_20_RC2_x_201)cocos2dx\support


"$(COCOS2D_20_RC2_x_201)cocos2dx\platform";"$(COCOS2D_20_RC2_x_201)cocos2dx\include";"$(COCOS2D_20_RC2_x_201)cocos2dx";
"$(COCOS2D_20_RC2_x_201)CocosDenshion\include";
"$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\OGLES";
"$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\libxml2";"$(COCOS2D_20_RC2_x_201)cocos2dx\platform\third_party\win32\iconv"

Tasks
1 预研 （2-3d）
   a）在cocos2d下显示三维模型（Box），能旋转，不考虑缩放，无贴图。（2d）
   b）为模型添加贴图。因cocos下有自己的贴图体系，工作模式需要仔细调研，使其能直接应用到三维模型上。（1d）
   c) Flickr API (NA,后续考虑)
2 3D support classes （12-13d）
   Math：Vector，Matrix，Frustum，BoundingBox，Quaternion等等。（4-5d）
   Mesh：模型结构定义，要同时考虑需要加载的模型和已经动态生成的切割后生成的模型。主要数据为：模型顶点数据，面数据，贴图坐标数据，包围盒数据等。（3d）
   Scene：场景相关程序。Camera（重要）；Light（可选）；Viewport；（5d）

3 3DS Mesh Loader （5-7d）
  加载3DS模型文件，参考irrlicht中加载程序。

4 完成切割算法 （10-15d）

5 Hologram 应用框架实现 (30d)
  a）基础控件：Button，Label，3D（Model Preview）List（重要）(5d)
  b）按设计完成应用程序框架。（logo -> model select <-> HOLO!）(15d)
  c) Flickr流程支持程序。(10d)

6 UI 调整，细化 （10d）

7 Test！（20d）

合计
1	2	3
2	14	16
3	19	23
4	29	38
5	59	68
6	69	78
7	89	98


6-16
地图再次成为障碍，当坐标回退的时候，地图生成程序无法正常工作。
正确的地图生成器应该是，根据视角的变化，扫描剔除（killMyself）不在视角中的精灵;同时生成可能进入视角的精灵。
或者，全树生成，进入视角，将要进入视角的精灵才更新，其他的精灵缓慢更新。
决定采用第二种方式。

CAStageLayer <- CASpaceTree
	<- _viewport
CASpaceTree < CANamedSprites
	管理四叉树，完成区域管理，更新管理等。
	update(viewport)
CASpaceNode <- CASprite(s)

map格式
<map name="name" follow="yes/no">
	<sprite mod="spr:percent;pat:percent[;spr...]" spos="" range="" />
</map>

6-15
onUpdate的诡异行为，最终决定抛弃。
CAStage 将使用 schedule Update，作为唯一的更新入口点

CAStage : CCLayer, CAScheduleNode
	CCBatchSprite(s) <- CASprite(s)
	CAStageLayer(s) <- CAStageLayer(s)
CAStageLayer : CASpriteContainer, CAAnimateObject, CAMachine
	CAStageLayer(s)
	CASprite(s)
CASprite : CCSprite, CASpriteContainer, CAAnimateObject, CAMachine
	CASprite(s)
	
6-13
timeline 对应后续的地图，我们要规划好地图的使用。
需要的功能：
1	activeMap	使给定的timeline有效。
1.5	updateMap	更新给定的timeline，继续根据当前View生成属于这个Map，可能被View的精灵。
2	destroyMap	将属于某个timeline的所有精灵移除。
3	showMap		将属于某个timeline的所有精灵显示或隐藏。


6-11
经过试验，发现多层次的Layer下面挂上对应相同贴图的BactchNode会造成渲染错误。
开始修改为StageLayer不再继承于CCLayer，而是单独的数据类，无论层次如何，大家一起使用Stage的Layer
将BatchNode生成维护改为在Stage中进行。
CAStageLayerBase ==> CCNode now.
:) OK!
渲染速度应该也会有一定的提高。

6-10，7月份进行这部分修正。目前暂时使用当前timeline模式。

stage中维护当前视角，
layer 根据当前视窗，生成必要的Node（这个过程最好在渲染过程之前完成，即Update过程），
按Node组织游戏世界空间。当Node移出视窗，则将其删除，并删除一切连带的节点中的精灵。
这个概念类似 Ogre3D的场景组织方式。

节点被删除，可以缓存到stage中，后续直接重新利用，不再重新分配内存。
精灵其实也是一样，也可以缓存。



StageLayer中的时间线模式有很多问题。
如何形成一个更加合理的地图模式？

序列资源生成，比如海浪，沙地，鱼，珍珠，属于这个游戏特殊的东西，建议是TS系列类生成。
	<layout name="fishes"> 这个分类先不增加，还是沿用以前的 timeline
		<timeline />
		<timeline />
	</layout>
固定位置资源生成，比如对话框，控件等等，属于普遍东西，使用CA系列类生成，在CAStageLayer中内建。
建立更加易于理解的函数甚至是类进行控制。
格式是：
	<layout name="dialog_pause">
		<control name="panel_back" spr/pat="" pos="" />
		<control name="button_resume" spr/pat="" pos="" />
		<control name="..." />
		<machine />
	</layout>

CAStageLayerLayout
	CAStageLayerLayout
	//this will create controls in dialog model
	static CAStageLayerLayout* create(CADialogModel* pmodel);
	//show or hide this layout
	void show(bool s = true);
	//destroy all sprites in this layout
	void destroy();

上述的想法变成了
CAStageLayer
	sub CAStageLayer(s)
	sub CAStageLayerLayout(s)
	sub CASprite(s)
这样的话，相当于产生了无用的语义重复层次，CAStageLayerLayout其实还是相当于一个Layer而已，可以使用sub Layer即可以解决问题。
鉴于上，我们应该强化对 sub Layers的管理。
我们增加方法管理sub Layer的行为即可。增加sublayer的返回数据管理。

同时增加新的精灵生成控制还是必要的,因为他不存在任何随机成分，格式理应更加简洁。
	<control name="panel_back" spr/pat="" pos="" />


规划界面之间跳转关系
logo + Loading (play)
play 
	state prepare
		等待0.5秒，各种元素 fadein
		站立的玩家，手托珍珠，周围的鱼
		大的Play按钮
			点击 Play按钮	进入play状态
			各种和游戏过程无关的精灵fadeout散去，有关的精灵移入正确位置，Player跃入，游戏开始
		上次最佳距离，持有的金币
		商店按钮	
			点击按钮		进入gearup状态
			商店列表移入，可以使用金币购买道具(一次性），在下次游戏中会自动出现
		静音按钮1，2
			点击，切换音效，音乐是否静音；游戏过程中依然存在
		Fliker，Facebook，Twitter
			目前没有
		广告
	state gearup
		有透明背景，有关闭按钮，有4个道具，有目前金币数量。
		
		加速道具，出现可骑乘的鱼，鱼种不同，一共5种（），速度挑战你的喜好不同，可以抵挡一次碰撞。
		减速道具，玩家被气泡包裹，速度在2秒内减低3档，持续5秒，速度在2秒内恢复。 
		吸金道具，玩家被金色气泡包裹，持续10秒，在一定范围内遇到珍珠，会吸附，并吃掉
		保险套道具，玩家被黑色气泡包裹，持续10秒，抵挡一次碰撞
		每种道具购买一次，出现几率增加20%，各种道具默认出现几率为20%，同一个道具可以购买多次，最多4次
		
		点击关闭，			进入 prepare状态
	state  play
		显示记分板，暂停按钮，玩家跃入，游戏开始
		玩家死亡，			进入 over 状态
		点击暂停按钮		进入 pause 状态
	state over
		显示 loading 进度条，重新加载 play界面，并进入 prepare界面
	state pause
		显示暂停对话框，重新开始，继续按钮
		重新开始			进入  prepare 状态
		继续				隐藏暂停对话框，继续
		
			

规划各种特殊控件的知名States

init
fadein
running
	[stand]
	[pressed]
[dying]
[paused]
fadeout
idle

这是可能是一次较大的改动（已经放弃）
发现几个弊端：
1 CCBatchSpriteNode的子Sprite只能使用同一贴图。致使无法构建理想的精灵层次关系。
2 CCBatchSpriteNode的子Sprite无法使用visit函数，进行必要的更新。
3 scheduleUpdate无法保证更新树状有序。

如下改动之目的，利用CCNode的层次管理关系解决上述3，2，1的问题。
将数据更新和渲染独立出来，建立两套层次管理结构。
一个用于更新数据（利用CCNode），一个用于渲染数据（使用cocos）

cocos层面：
	我们只生成一个 CCLayer，对应我们的Stage。
	其他的都是CCSprite，隶属于CCLayer下的CCBatchSpriteNode。
	CCSprite之间没有层次关系，即我们不使用cocos的层次组织关系。
	原因是由于Batch的存在，树状结构中无法同时出现不同贴图的CCSprite，
	并不同的CCSprite存在在不同的层次中。（重大弊端！）
	各个Cache管理的抽象层。
	应用程序框架的抽象层。
cae层面：
	一个World，下属N个Stage，可以互相切换，利用cocos方法。
	Stage中可以存放具有层次关系的Sprite，具有Timeline。
	Sprite有 AnchorPoint，Position, Scale, BoundingBox, Alpha, Visible, Poses等概念。
	当然也有Children也是Sprite。这样极其容易构建Dialog，以及其他控件。
	每个Sprite有可能生成一个（不是继承）CCSprite放到Stage对应的CCLayer->CCBatchSpriteNode中。
	坐标由Sprite决定。
	
	Stage，Sprite都从 CCNode继承，借用其层次管理方法（可行否？）。负责父子关系，渲染队列等等。
经过试验，在CCNode::visit过程中会引发CCNode::draw调用，此时矩阵已经被更新。
将Sprite对应的CCNode的矩阵传递给cocos层面的CCSprite很困难。

那么我们只能使用另外一个方法，假设我们构建的CAXXX对象都是从一个接口中继承而来。
那么我们在CAStage中遍历所有的Child，强制成CAXXX接口对象，调用其update方法，完成树状更新。
但是由于CCBatchSpriteNode已经强制了树状结构畸形，树状更新已经不具意义。
所以，我想还是只是进行第一步，时间概念的统一，在CAStage中建立统一的时间体系。
更新依旧使用sheduleUpdate的批量更新方式。
	
如何实现精灵之间的层次关系呢？目前不行。
只能硬编码，假设层次关系，多编制一些代码。
太累了，唉。还是进一步调研一下CCNode中的矩阵用法吧。

另外一个方法是取消 CCSpriteBatchNode用法，使用普通的Sprite，不知道性能差别会多大。

决定，忽略层次关系！大家都是兄弟！！！



* 程序中出现的一切静态成员变量必须是指针，一次保证Mem Guarder的变量实例能最后被释放。

	int index;
	CCObject* pkey;

	CAXMLNodes* pnodes = pnode->nodes("anim");
	_Assert(null != pnodes);
	_anims.init(pnodes->count());
	index = 0;
	CCARRAY_FOREACH(pnodes, pkey)
	{
		CAXMLNode* pnodeAnim = (CAXMLNode*)pkey;
		
		_anims[index].ref = pnodeAnim->attr("ref");
		float duration;
		CAString::toFloat(pnodeAnim->attr("duration"), duration);
		_anims[index].duration = duration / 1000.0f;
	}

一个精灵可以由多个mesh，哪些mesh当前被使用取决于pose。

一个Pose应该由多个part组成
	为了统一控制，显示存在一个总part，然后是多个子part，子part仍然可以有子part无限级别。
	总part对应着当前精灵，当总part位置被修改的时候，所有的子part都跟随联动。
	part必须名字，总part默认名字是root。总part是个容器。不对应任何mesh。
每个part对应一个mesh（总part没有mesh），这个动画的总时间可以被重载，每个part可以存在一个modifier，也可以没有
modifier是由一组key组成，key中描述对part的各种动态设定，按timeline描述，运行过程中插值完成取值。

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
2012-4-22
坐标体系，使用游戏坐标，Layer记录总偏移 _posOffset和总速度 _speed，Sprite重载ccPoint _posGame;
当onUpdate发生的时候，_speed 更新_posGame, 使用 _posOffset 和 _posGame 计算屏幕坐标，使用 CCSprite::setPosition设置渲染位置
各种精灵，根据自己的情况，调用killMyself决定自己的生命周期。

在CASprite中增加onRender在这个过程中，利用各种制约变量计算CCSprite的setPosition,setRotation等等。

Layer更新过程，onUpdate
根据_posOffset, 生成屏幕右侧一屏的新精灵，针对地表，水草，宝石，我们可以专门配置一个Generator的类处理。
Generator中有难度，当前距离等概念。利用xml配置动态生成过程。
各种pattern，所生成的各种精灵都是速度为0，0的，即 _posGame 不会改变，当onUpdate的时候被Layer::_posOffset制约。
其他后面Layer中产生的修饰物品的速度，介于0和Player速度之间。

将Action修改成自己管理。自己主动调用update，依赖Sprite::onUpdate。
onUpdate调用Action->onUpdate(this as CASprite*) 得到 pkey，
当action是abs的时候
	使用 pkey信息，更新所涉及的精灵的绝对位置 _posGame
当action非abs的时候
	使用 pkey信息，更新所涉及的精灵的相对位置 _posGame, Action开始的时候会记录相对基准数据。

LayerBackground中的sea，light，他们的速度是当前游戏Player的水平速度，他们的位置一直在变化。

1 将Action转换成不依赖cocos的，使程序更加易于控制，更加简单。（现在能更新数据的切入点太多，很难同步思考）
2 建立真实游戏坐标体系。（目前的坐标系很混乱）
3 将游戏场景中游戏过程可能出现的各种事物搭建齐全。（人物，鱼，珍珠，海浪，沙地，水草）
4 辅助场景生成算法。（难度系数，速度，场景复杂度，各个时间段中各种事物出现的几率）
5 搭建全部界面的简易框架。（重点，简易！粗糙不怕，要全面）
如果还有时间
1 添加xml变量设置程序。（简单，4小时搞定）
2 添加游戏局部存储数据的方法。（简单）

xml设置程序，依赖layer，spr中的各种状态定义。类似Flex的思考模式
结构如下：
<states>
	<state name="default">
		<key name="speed">0,0</key>
	</state>
	<state name="preparing">
		<key name="speed">0,0</key>
	</state>
	<state name="play">
		<key name="speed">100,0</key>
	</state>
</states>

Layer offset 被各个可渲染的单位用于计算GL位置。

Sprite:
	x = _posGame.x - _pLayer->offset.x;
	World2GL(x) ==> glx, CCSprite::setPositionX(x);

SpriteSubMesh:
	此对象的一切坐标都是相对于Sprite
	x = _posGame.x + pspr->_posGame.x - pspr->_pLayer->offset.x;
	World2GL(x) ==> glx, CCSprite::setPositionX(x)

鉴于上述推定，_posGame 应该在SpriteSubMesh中设计。

产生规则：所有SubMesh只能相对Sprite设计。

Layer speed，Layer本身并不渲染，所以Layer speed只是影响Layer offset的一个因素。

Action
因为Controller会相对基准动作影响位置，其他修改位置的过程和这个过程可能冲突。需要思考。

以上都错了。

Layer其实是一个View，只是决定了显示窗口是什么，在哪里开始显示，自己本身不受任何物件的速度影响。
本游戏，Layer offset 由player的当前位置反向映射到根Layer再到Stage。Layer只是负责承载不同的前后关系。
本身不保存任何除了zorder之外的数据。

现在Action已经完全是相对动作。目前需要思考的是Action和Sprite，SpriteSubMesh的结合问题。
SpriteSubMesh只是针对Sprite相对变化。
Action永远相对于Sprite的（0，0）位置按时间变化，所以这个变化值应该加上精灵的当前位置。

修改所有的spr文件，保证坐标使用游戏坐标。

Action "pos" 是游戏坐标
Action "yardpos" 是像素坐标


stage offset for player
Sprite.setStageOffsetIgnoreFlag(true/false)
for background


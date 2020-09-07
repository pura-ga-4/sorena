
# include <Siv3D.hpp>

// シーンの名前
enum class State
{
    Title,
    Game,
    endkard,
    aitm
};

// ゲームデータ
struct GameData
{
    // ハイスコア
    int32 highScore = 0;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;

// タイトルシーン
class Title : public MyApp::Scene
{
private:

    Rect m_startButton = Rect(Arg::center = Scene::Center().movedBy(0, 0), 300, 60);
    Transition m_startTransition = Transition(0.4s, 0.2s);

    Rect m_exitButton = Rect(Arg::center = Scene::Center().movedBy(0, 100), 300, 60);
    Transition m_exitTransition = Transition(0.4s, 0.2s);

public:

    Title(const InitData& init)
        : IScene(init) {}

    void update() override
    {
        m_startTransition.update(m_startButton.mouseOver());
        m_exitTransition.update(m_exitButton.mouseOver());

        if (m_startButton.mouseOver() || m_exitButton.mouseOver())
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }

        if (m_startButton.leftClicked())
        {
            changeScene(State::Game);
        }

        if (m_exitButton.leftClicked())
        {
            System::Exit();
        }
    }

    void draw() const override
    {
        const String titleText = U"ブロックくずし";
        const Vec2 center(Scene::Center().x, 120);
        FontAsset(U"Title")(titleText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
        FontAsset(U"Title")(titleText).drawAt(center);

        m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
        m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

        FontAsset(U"Menu")(U"はじめる").drawAt(m_startButton.center(), ColorF(0.0, 1,0));
        FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.0, 1,0));

        Rect(0, 500, Scene::Width(), Scene::Height() - 500)
            .draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

        const int32 highScore = getData().highScore;
        FontAsset(U"Score")(U"High score: {}"_fmt(highScore)).drawAt(Vec2(620, 550));
    }
};

class aitm : public MyApp::Scene
{
private:
    Circle m_bb = Circle(400, 350, 8);
    int32 sos = 375.0;
    Vec2 m_bbV = Vec2(0, -sos);

public:
    void update() override
    {
        m_bb.moveBy(m_bbV * Scene::DeltaTime());
    }

    void draw() const override
    {
        m_bb.draw(Palette::Orange);
    }


};

// ゲームシーン
class Game : public MyApp::Scene
{
private:


    // ブロックのサイズ
    static constexpr Size blockSize = Size(40, 20);
     
    // ボールの速さ
    static constexpr double speed = 300.0;

    // ブロックの配列
    Array<Rect> m_blocks;

    // ボールの速度
    Vec2 m_ballVelocity = Vec2(0, -speed);

    // ボール
    Circle m_ball = Circle(400, 400, 8);

    // パドル
    Rect m_paddle = Rect(Arg::center(Cursor::Pos().x, 500), 100, 10);

    // スコア
    int32 m_score = 0;
    int32 m_tp = 0;


    //アイテムの生成
    static constexpr Size m_tS = Size(20, 10);
    Array<Rect> m_t;


    //ボール(1)
    Circle m_bb = Circle(400, 350, 8);
    int32 sos = 325.0;
    Vec2 m_bbV = Vec2(0, -sos);

    //ボール(2)
    Circle m_b2 = Circle(400, 350, 8);
    Vec2 m_b2V= Vec2(0, -sos);
    
    //ボール(3)
    Circle m_b3 = Circle(400, 350, 8);
    Vec2 m_b3V = Vec2(0, -sos);

    //ボール(4)
    Circle m_b4 = Circle(400, 350, 8);
    Vec2 m_b4V = Vec2(0, -sos);

    //ボール(5)
    Circle m_b5 = Circle(400, 350, 8);
    Vec2 m_b5V = Vec2(0, -sos);

    //ボール(6)
    Circle m_b6 = Circle(400, 350, 8);
    Vec2 m_b6V = Vec2(0, -sos);

    //ボール(7)
    Circle m_b7 = Circle(400, 350, 8);
    Vec2 m_b7V = Vec2(0, -sos);

    //ボール(8)
    Circle m_b8 = Circle(400, 350, 8);
    Vec2 m_b8V = Vec2(0, -sos);

    //ボール(9)
    Circle m_b9 = Circle(400, 350, 8);
    Vec2 m_b9V = Vec2(0, -sos);

    //ボール(10)
    Circle m_b10 = Circle(400, 350, 8);
    Vec2 m_b10V = Vec2(0, -sos);

    


public:
   

    Game(const InitData& init)
        : IScene(init)
    {
       for (int32 i = 0; i <10; ++i)
       {
           m_t << Rect(Random(760), Random(200),m_tS);
        };


         // 横 (Scene::Width() / blockSize.x) 個、縦 7 個のブロックを配列に追加する
        for (auto p : step(Size((Scene::Width() / blockSize.x), 7)))
        {
                        m_blocks << Rect(p.x * blockSize.x, 60 + p.y * blockSize.y, blockSize);
         };
    };

    void update() override
    {
        // パドルを操作
        m_paddle = Rect(Arg::center(Cursor::Pos().x, 500), 100, 10);

        // ボールを移動
        m_ball.moveBy(m_ballVelocity * Scene::DeltaTime());



        // ブロックを順にチェック
        for (auto it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            // ボールとブロックが交差していたら
            if (it->intersects(m_ball))
            {

                (it->bottom().intersects(m_ball) || it->top().intersects(m_ball) ? m_ballVelocity.y : m_ballVelocity.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                ++m_score;

                // これ以上チェックしない  
                break;
            }

            // ボールbとブロックが交差していたら
            if (it->intersects(m_bb))
            {

                (it->bottom().intersects(m_bb) || it->top().intersects(m_bb) ? m_bbV.y : m_bbV.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            //ボール２
            if (it->intersects(m_b2))
            {

                (it->bottom().intersects(m_b2) || it->top().intersects(m_b2) ? m_b2V.y : m_b2V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }
           
            //ボール3
            if (it->intersects(m_b3))
            {

                (it->bottom().intersects(m_b3) || it->top().intersects(m_b3) ? m_b3V.y : m_b3V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score +2;

                // これ以上チェックしない  
                break;
            }

            // ボール4
            if (it->intersects(m_b4))
            {

                (it->bottom().intersects(m_b4) || it->top().intersects(m_b4) ? m_b4V.y : m_b4V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            // ボール5
            if (it->intersects(m_b5))
            {

                (it->bottom().intersects(m_b5) || it->top().intersects(m_b5) ? m_b5V.y : m_b5V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            //ボール6
            if (it->intersects(m_b6))
            {

                (it->bottom().intersects(m_b6) || it->top().intersects(m_b6) ? m_b6V.y : m_b6V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            // ボール7
            if (it->intersects(m_b7))
            {

                (it->bottom().intersects(m_b7) || it->top().intersects(m_b7) ? m_b7V.y : m_b7V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            // ボール8
            if (it->intersects(m_b8))
            {

                (it->bottom().intersects(m_b8) || it->top().intersects(m_b8) ? m_b8V.y : m_b8V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            //ボール9
            if (it->intersects(m_b9))
            {

                (it->bottom().intersects(m_b9) || it->top().intersects(m_b9) ? m_b9V.y : m_b9V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }

            // ボール10
            if (it->intersects(m_b10))
            {

                (it->bottom().intersects(m_b10) || it->top().intersects(m_b10) ? m_b10V.y : m_b10V.x) *= -1;

                // ブロックを配列から削除（イテレータが無効になるので注意）
                m_blocks.erase(it);

                // スコアを加算
                m_score = m_score + 2;

                // これ以上チェックしない  
                break;
            }
        }
        
        //アイテムとボールのが交差したら
        for (auto a = m_t.begin(); a != m_t.end(); ++a)
        {
            if (a->intersects(m_ball))
            {
                //ボールが跳ね返る
                (a->bottom().intersects(m_ball) || a->top().intersects(m_ball) ? m_ballVelocity.y : m_ballVelocity.x) *= -1;
                
                //アイテムを消す
                m_t.erase(a);

                ++m_tp;
                //ここにaitmシーンを追加したい

                //これ以上チェックしない
                break;
            }

        
       
        }
        
        if (m_tp >= 1)
        {
            
            m_bb.draw(Palette::Orange);
            m_bb.moveBy(m_bbV * Scene::DeltaTime());

        }
        if (m_tp >= 2)
        {

            m_b2.draw(Palette::Orange);
            m_b2.moveBy(m_b2V * Scene::DeltaTime());

        }
        if (m_tp >= 3)
        {

            m_b3.draw(Palette::Orange);
            m_b3.moveBy(m_b3V * Scene::DeltaTime());

        }
        if (m_tp >= 4)
        {

            m_b4.draw(Palette::Orange);
            m_b4.moveBy(m_b4V * Scene::DeltaTime());

        }
        if (m_tp >= 5)
        {

            m_b5.draw(Palette::Orange);
            m_b5.moveBy(m_b5V * Scene::DeltaTime());

        }
        if (m_tp >= 6)
        {

            m_b6.draw(Palette::Orange);
            m_b6.moveBy(m_b6V * Scene::DeltaTime());

        }
        if (m_tp >= 7)
        {

            m_b7.draw(Palette::Orange);
            m_b7.moveBy(m_b7V * Scene::DeltaTime());

        }
        if (m_tp >= 8)
        {

            m_b8.draw(Palette::Orange);
            m_b8.moveBy(m_b8V * Scene::DeltaTime());

        }
        if (m_tp >= 9)
        {

            m_b9.draw(Palette::Orange);
            m_b9.moveBy(m_b9V * Scene::DeltaTime());

        }
        if (m_tp >= 10)
        {

            m_b10.draw(Palette::Orange);
            m_b10.moveBy(m_b10V * Scene::DeltaTime());

        }

        // 天井にぶつかったらはね返る
        if (m_ball.y < 0 && m_ballVelocity.y < 0)
        {
            m_ballVelocity.y *= -1;
        }

        if (m_ball.y > Scene::Height())
        {
            changeScene(State::Title);
            getData().highScore = Max(getData().highScore, m_score);
        }

        // 左右の壁にぶつかったらはね返る
        if ((m_ball.x < 0 && m_ballVelocity.x < 0) || (Scene::Width() < m_ball.x && m_ballVelocity.x > 0))
        {
            m_ballVelocity.x *= -1;
        }

        // パドルにあたったらはね返る
        if (m_ballVelocity.y > 0 && m_paddle.intersects(m_ball))
        {
            // パドルの中心からの距離に応じてはね返る向きを変える
            m_ballVelocity = Vec2((m_ball.x - m_paddle.center().x) * 5, -m_ballVelocity.y).setLength(speed);
        }
    
        
          //ボールb 天井にぶつかったらはね返る
        if (m_bb.y < 0 && m_bbV.y < 0)
        {
            m_bbV.y *= -1;
        }
        // ボールb左右の壁にぶつかったらはね返る
        if ((m_bb.x < 0 && m_bbV.x < 0) || (Scene::Width() < m_bb.x && m_bbV.x > 0))
        {
            m_bbV.x *= -1;
        }
        // ボールbパドルにあたったらはね返る
        if (m_bbV.y > 0 && m_paddle.intersects(m_bb))
        {
            // ボールbパドルの中心からの距離に応じてはね返る向きを変える
            m_bbV = Vec2((m_bb.x - m_paddle.center().x) * 5, -m_bbV.y).setLength(speed);
        }

        //ボール２ 天井にぶつかったらはね返る
        if (m_b2.y < 0 && m_b2V.y < 0)
        {
            m_b2V.y *= -1;
        }
        // ボール2左右の壁にぶつかったらはね返る
        if ((m_b2.x < 0 && m_b2V.x < 0) || (Scene::Width() < m_b2.x && m_b2V.x > 0))
        {
            m_b2V.x *= -1;
        }
        // ボール2パドルにあたったらはね返る
        if (m_b2V.y > 0 && m_paddle.intersects(m_b2))
        {
            // ボールbパドルの中心からの距離に応じてはね返る向きを変える
            m_b2V = Vec2((m_b2.x - m_paddle.center().x) * 5, -m_b2V.y).setLength(speed);
        }

        //ボール3 天井にぶつかったらはね返る
        if (m_b3.y < 0 && m_b3V.y < 0)
        {
            m_b3V.y *= -1;
        }
        // ボール3左右の壁にぶつかったらはね返る
        if ((m_b3.x < 0 && m_b3V.x < 0) || (Scene::Width() < m_b3.x && m_b3V.x > 0))
        {
            m_b3V.x *= -1;
        }
        // ボール3パドルにあたったらはね返る
        if (m_b3V.y > 0 && m_paddle.intersects(m_b3))
        {
            // ボール3パドルの中心からの距離に応じてはね返る向きを変える
            m_b3V = Vec2((m_b3.x - m_paddle.center().x) * 5, -m_b3V.y).setLength(speed);
        }

        //ボール4天井にぶつかったらはね返る
        if (m_b4.y < 0 && m_b4V.y < 0)
        {
            m_b4V.y *= -1;
        }
        // ボール4左右の壁にぶつかったらはね返る
        if ((m_b4.x < 0 && m_b4V.x < 0) || (Scene::Width() < m_b4.x && m_b4V.x > 0))
        {
            m_b4V.x *= -1;
        }
        // ボール4パドルにあたったらはね返る
        if (m_b4V.y > 0 && m_paddle.intersects(m_b4))
        {
            // ボール4パドルの中心からの距離に応じてはね返る向きを変える
            m_b4V = Vec2((m_b4.x - m_paddle.center().x) * 5, -m_b4V.y).setLength(speed);
        }

        //ボール5 天井にぶつかったらはね返る
        if (m_b5.y < 0 && m_b5V.y < 0)
        {
            m_b5V.y *= -1;
        }
        // ボール5左右の壁にぶつかったらはね返る
        if ((m_b5.x < 0 && m_b5V.x < 0) || (Scene::Width() < m_b5.x && m_b5V.x > 0))
        {
            m_b5V.x *= -1;
        }
        // ボール5パドルにあたったらはね返る
        if (m_b5V.y > 0 && m_paddle.intersects(m_b5))
        {
            // ボール5パドルの中心からの距離に応じてはね返る向きを変える
            m_b5V = Vec2((m_b5.x - m_paddle.center().x) * 5, -m_b5V.y).setLength(speed);
        }

        //ボール6 天井にぶつかったらはね返る
        if (m_b6.y < 0 && m_b6V.y < 0)
        {
            m_b6V.y *= -1;
        }
        // ボール6左右の壁にぶつかったらはね返る
        if ((m_b6.x < 0 && m_b6V.x < 0) || (Scene::Width() < m_b6.x && m_b6V.x > 0))
        {
            m_b6V.x *= -1;
        }
        // ボール6パドルにあたったらはね返る
        if (m_b6V.y > 0 && m_paddle.intersects(m_b6))
        {
            // ボール6パドルの中心からの距離に応じてはね返る向きを変える
            m_b6V = Vec2((m_b6.x - m_paddle.center().x) * 5, -m_b6V.y).setLength(speed);
        }

        //ボール7 天井にぶつかったらはね返る
        if (m_b7.y < 0 && m_b7V.y < 0)
        {
            m_b7V.y *= -1;
        }
        // ボール7左右の壁にぶつかったらはね返る
        if ((m_b7.x < 0 && m_b7V.x < 0) || (Scene::Width() < m_b7.x && m_b7V.x > 0))
        {
            m_b7V.x *= -1;
        }
        // ボール7パドルにあたったらはね返る
        if (m_b7V.y > 0 && m_paddle.intersects(m_b7))
        {
            // ボール7パドルの中心からの距離に応じてはね返る向きを変える
            m_b7V = Vec2((m_b7.x - m_paddle.center().x) * 5, -m_b7V.y).setLength(speed);
        }

        //ボール8天井にぶつかったらはね返る
        if (m_b8.y < 0 && m_b8V.y < 0)
        {
            m_b8V.y *= -1;
        }
        // ボール8左右の壁にぶつかったらはね返る
        if ((m_b8.x < 0 && m_b8V.x < 0) || (Scene::Width() < m_b8.x && m_b8V.x > 0))
        {
            m_b8V.x *= -1;
        }
        // ボール8パドルにあたったらはね返る
        if (m_b8V.y > 0 && m_paddle.intersects(m_b8))
        {
            // ボール8パドルの中心からの距離に応じてはね返る向きを変える
            m_b8V = Vec2((m_b8.x - m_paddle.center().x) * 5, -m_b8V.y).setLength(speed);
        }

        //ボール9 天井にぶつかったらはね返る
        if (m_b9.y < 0 && m_b9V.y < 0)
        {
            m_b9V.y *= -1;
        }
        // ボール9左右の壁にぶつかったらはね返る
        if ((m_b9.x < 0 && m_b9V.x < 0) || (Scene::Width() < m_b9.x && m_b9V.x > 0))
        {
            m_b9V.x *= -1;
        }
        // ボール9パドルにあたったらはね返る
        if (m_b9V.y > 0 && m_paddle.intersects(m_b9))
        {
            // ボール9パドルの中心からの距離に応じてはね返る向きを変える
            m_b9V = Vec2((m_b9.x - m_paddle.center().x) * 5, -m_b9V.y).setLength(speed);
        }

        //ボール10天井にぶつかったらはね返る
        if (m_b10.y < 0 && m_b10V.y < 0)
        {
            m_b10V.y *= -1;
        }
        // ボール10左右の壁にぶつかったらはね返る
        if ((m_b10.x < 0 && m_b10V.x < 0) || (Scene::Width() < m_b10.x && m_b10V.x > 0))
        {
            m_b10V.x *= -1;
        }
        // ボール10パドルにあたったらはね返る
        if (m_b10V.y > 0 && m_paddle.intersects(m_b10))
        {
            // ボール10パドルの中心からの距離に応じてはね返る向きを変える
            m_b10V = Vec2((m_b10.x - m_paddle.center().x) * 5, -m_b10V.y).setLength(speed);
        }
    }

    void draw() const override
    {
       FontAsset(U"Score")(m_score).drawAt(Scene::Center().x, 30);

        // すべてのブロックを描画する
        for (const auto& block : m_blocks)
        {
            block.stretched(-1).draw(HSV(block.y - 40));
        }
        
        for (const auto& block : m_t)
        {
            block.stretched(-1).draw(HSV(block.y - 40));
        }
        // ボールを描く
        m_ball.draw();

        // パドルを描く
        m_paddle.draw();

        


    }
};





class endkard : public MyApp::Scene
{
private:

    Rect m_startButton = Rect(Arg::center = Scene::Center().movedBy(0, 0), 300, 60);
    Transition m_startTransition = Transition(0.4s, 0.2s);

    Rect m_exitButton = Rect(Arg::center = Scene::Center().movedBy(0, 100), 300, 60);
    Transition m_exitTransition = Transition(0.4s, 0.2s);

public:
   endkard(const InitData& init)
        : IScene(init) {}

    void update() override
    {
        m_startTransition.update(m_startButton.mouseOver());
        m_exitTransition.update(m_exitButton.mouseOver());

        if (m_startButton.mouseOver() || m_exitButton.mouseOver())
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }

        if (m_startButton.leftClicked())
        {
            changeScene(State::Game);
        }

        if (m_exitButton.leftClicked())
        {
            System::Exit();
        }
    }

    void draw() const override
    {
        const String titleText = U"ブロックくずし";
        const Vec2 center(Scene::Center().x, 120);
        FontAsset(U"Title")(titleText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
        FontAsset(U"Title")(titleText).drawAt(center);

        m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
        m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

        FontAsset(U"Menu")(U"タイトルへ").drawAt(m_startButton.center(), ColorF(0.0, 1, 0));
        FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.0, 1, 0));

        Rect(0, 500, Scene::Width(), Scene::Height() - 500)
            .draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

        const int32 highScore = getData().highScore;
        FontAsset(U"Score")(U"High score: {}"_fmt(highScore)).drawAt(Vec2(620, 550));
    }
};




void Main()
{
   
    // 使用するフォントアセットを登録
    FontAsset::Register(U"Title", 120, U"example/font/AnnyantRoman/AnnyantRoman.ttf");
    FontAsset::Register(U"Menu", 30, Typeface::Regular);
    FontAsset::Register(U"Score", 36, Typeface::Bold);

    // 背景色を設定
    Scene::SetBackground(ColorF(0.0, 0,0));

    // シーンと遷移時の色を設定
    MyApp manager;
    manager
        .add<Title>(State::Title)
        .add<Game>(State::Game)
        .setFadeColor(ColorF(1.0));

    while (System::Update())
    {
        if (!manager.update())
        {
            break;
        }
       
    }

}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//

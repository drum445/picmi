/* *************************************************************************
 *  Copyright 2012 Jakob Gruber                                            *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ************************************************************************* */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kxmlguiwindow.h>
#include <QTimer>
#include <highscore/kscoredialog.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kgamedifficulty.h>

#include "src/logic/picmi.h"
#include "scene.h"
#include "view.h"

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void startGame();
    void togglePaused(bool paused);
    void settings();
    void gameWon();
    void undo();
    void about();
    void highscores();
    void levelChanged(KGameDifficulty::standardLevel level);
    void customLevelChanged(int level);

private:
    void restoreWindowState();
    void saveWindowState();
    void pauseGame();
    std::shared_ptr<KScoreDialog> createScoreDialog();
    void setupActions();

    KAction *m_action_undo;
    KToggleAction *m_action_pause;
    View m_view;
    std::shared_ptr<Picmi> m_game;
    std::shared_ptr<Scene> m_scene;
    QTimer m_timer;

    const QString m_key_size, m_key_pos;

    bool m_in_progress;
};

#endif // MAINWINDOW_H

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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QGraphicsSimpleTextItem>

#include "highscorewindow.h"
#include "helpwindow.h"
#include "settingswindow.h"
#include "config.h"
#include "src/constants.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_key_size("window/size"), m_key_pos("window/position")
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setApplicationName("picmi");

    setWindowIcon(QIcon(QString(FILEPATH) + "icon.png"));

    restoreWindowState();

    m_scores.reset(new HighScores);

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(startGame()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
    connect(ui->actionPause, SIGNAL(triggered(bool)), this, SLOT(togglePaused(bool)));
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo()));

    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateStatusbar()));
    m_timer.start();

    startGame();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveWindowState();
    QMainWindow::closeEvent(event);
}

void MainWindow::saveWindowState() {
    QSettings settings;
    settings.setValue(m_key_size, size());
    settings.setValue(m_key_pos, pos());
    settings.sync();
}

void MainWindow::restoreWindowState() {
    QSettings settings;
    QSize s = settings.value(m_key_size, size()).toSize();
    QPoint p = settings.value(m_key_pos, pos()).toPoint();

    resize(s);
    move(p);
}

void MainWindow::updateStatusbar() {
    QTime elapsed = m_game->elapsedTime();
    ui->statusBar->showMessage(elapsed.toString());
}

void MainWindow::undo() {
    m_game->undo();
    m_scene->refresh();
}

void MainWindow::startGame() {

    if (m_scene) {
        disconnect(m_scene.get(), SIGNAL(gameWon()), this, SLOT(gameWon()));
    }

    ui->actionPause->setEnabled(true);
    ui->actionPause->setChecked(false);

    boost::shared_ptr<Settings> settings(new Settings());
    m_game.reset(new Picmi(settings));
    m_scene = ui->graphicsView->createScene(m_game);

    ui->graphicsView->setEnabled(true);
    ui->statusBar->clearMessage();

    connect(m_scene.get(), SIGNAL(gameWon()), this, SLOT(gameWon()));
}

void MainWindow::gameWon() {
    boost::shared_ptr<HighScore> score = m_game->endGame();
    m_scores->add(score);
    ui->graphicsView->setEnabled(false);
    ui->actionPause->setEnabled(false);

    HighScoreWindow w(m_scores, score, this);
    w.exec();
}

void MainWindow::togglePaused(bool paused) {
    ui->graphicsView->setPaused(paused);

    if (paused) {
        m_timer.stop();
    } else {
        m_timer.start();
    }
}

void MainWindow::help() {
    HelpWindow w(this);
    w.exec();
}

void MainWindow::settings() {
    SettingsWindow w(this);
    w.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

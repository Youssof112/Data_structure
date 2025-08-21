#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include "project.cpp"  // Put your existing code in this header file
#include <vector>

class LeaderboardGUI : public QWidget {
    Q_OBJECT

private:
    segment_tree tree;
    QTableWidget *tableWidget; // Promoted to a class member

    // Insert inputs
    QLineEdit *scoreInput;
    QLineEdit *finishTimeInput;

    // Query by time inputs
    QLineEdit *startTimeInput;
    QLineEdit *endTimeInput;

    // Query by ID inputs
    QLineEdit *startIdInput;
    QLineEdit *endIdInput;

    // Update inputs
    QLineEdit *updateIdInput;
    QLineEdit *updateScoreInput;
    QLineEdit *updateFinishInput;

public:
    LeaderboardGUI(QWidget *parent = nullptr)
        : QWidget(parent), tree(1000) // Set max players = 1000 for now
    {
        setWindowTitle("Time-Range Leaderboard Query System");
        setMinimumSize(500, 600);

        QVBoxLayout *mainLayout = new QVBoxLayout;

        QHBoxLayout *table = new QHBoxLayout;
        QHBoxLayout *fields = new QHBoxLayout;

        // --- Table Layout ---
        tableWidget = new QTableWidget; // Initialize the member widget
        tableWidget->setColumnCount(3);
        tableWidget->setHorizontalHeaderLabels({"Player ID", "Score", "Finish Time"});
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Make table read-only
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Make columns fit window
        table->addWidget(tableWidget);

        // --- Insert Section ---
        QGroupBox *insertBox = new QGroupBox("Insert Player");
        QVBoxLayout *insertLayout = new QVBoxLayout;
        scoreInput = new QLineEdit; scoreInput->setPlaceholderText("Score");
        finishTimeInput = new QLineEdit; finishTimeInput->setPlaceholderText("Finish Time");
        QPushButton *insertButton = new QPushButton("Insert Player");
        insertLayout->addWidget(scoreInput);
        insertLayout->addWidget(finishTimeInput);
        insertLayout->addWidget(insertButton);
        insertBox->setLayout(insertLayout);
        fields->addWidget(insertBox);

        // --- Query by Time Section ---
        QGroupBox *queryTimeBox = new QGroupBox("Query Top Player by Time");
        QVBoxLayout *queryTimeLayout = new QVBoxLayout;
        startTimeInput = new QLineEdit; startTimeInput->setPlaceholderText("Start Time");
        endTimeInput = new QLineEdit; endTimeInput->setPlaceholderText("End Time");
        QPushButton *queryTimeButton = new QPushButton("Query by Time");
        queryTimeLayout->addWidget(startTimeInput);
        queryTimeLayout->addWidget(endTimeInput);
        queryTimeLayout->addWidget(queryTimeButton);
        queryTimeBox->setLayout(queryTimeLayout);
        fields->addWidget(queryTimeBox);

        // --- Query by ID Section ---
        QGroupBox *queryIdBox = new QGroupBox("Query Top Player by ID");
        QVBoxLayout *queryIdLayout = new QVBoxLayout;
        startIdInput = new QLineEdit; startIdInput->setPlaceholderText("Start Player ID");
        endIdInput = new QLineEdit; endIdInput->setPlaceholderText("End Player ID");
        QPushButton *queryIdButton = new QPushButton("Query by ID");
        queryIdLayout->addWidget(startIdInput);
        queryIdLayout->addWidget(endIdInput);
        queryIdLayout->addWidget(queryIdButton);
        queryIdBox->setLayout(queryIdLayout);
        fields->addWidget(queryIdBox);

        // --- Update Section ---
        QGroupBox *updateBox = new QGroupBox("Update Player");
        QVBoxLayout *updateLayout = new QVBoxLayout;
        updateIdInput = new QLineEdit; updateIdInput->setPlaceholderText("Player ID");
        updateScoreInput = new QLineEdit; updateScoreInput->setPlaceholderText("New Score");
        updateFinishInput = new QLineEdit; updateFinishInput->setPlaceholderText("New Finish Time");
        QPushButton *updateButton = new QPushButton("Update Player");
        updateLayout->addWidget(updateIdInput);
        updateLayout->addWidget(updateScoreInput);
        updateLayout->addWidget(updateFinishInput);
        updateLayout->addWidget(updateButton);
        updateBox->setLayout(updateLayout);
        fields->addWidget(updateBox);

        // Set layout
        mainLayout->addLayout(table);
        mainLayout->addLayout(fields);

        setLayout(mainLayout);

        // --- Connect Buttons ---
        connect(insertButton, &QPushButton::clicked, this, &LeaderboardGUI::onInsertPlayer);
        connect(queryTimeButton, &QPushButton::clicked, this, &LeaderboardGUI::onQueryByTime);
        connect(queryIdButton, &QPushButton::clicked, this, &LeaderboardGUI::onQueryById);
        connect(updateButton, &QPushButton::clicked, this, &LeaderboardGUI::onUpdatePlayer);
    }

private:
    void refreshTable() {
        tableWidget->clearContents();
        tableWidget->setRowCount(0);

        vector<player_data> all_players = tree.get_all_players();
        tableWidget->setRowCount(all_players.size());

        int row = 0;
        for (const auto& player : all_players) {
            // Create non-editable table items
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(player.player_id));
            QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(player.score));
            QTableWidgetItem *timeItem = new QTableWidgetItem(QString::number(player.finish_time));

            tableWidget->setItem(row, 0, idItem);
            tableWidget->setItem(row, 1, scoreItem);
            tableWidget->setItem(row, 2, timeItem);
            row++;
        }
    }

private slots:
    void onInsertPlayer() {
        int score = scoreInput->text().toInt();
        int finishTime = finishTimeInput->text().toInt();

        tree.insert_into_tree(player_data(score, -1, finishTime));
        QMessageBox::information(this, "Insert Success", "Player inserted successfully!");
        refreshTable();
    }

    void onQueryByTime() {
        int start = startTimeInput->text().toInt();
        int end = endTimeInput->text().toInt();

        player_data top = tree.query_the_tree_by_time(start, end);
        if (top.score == -1)
            QMessageBox::information(this, "Result", "No player found in this range.");
        else
            QMessageBox::information(this, "Result",
                                     "Top Player:\nID: " + QString::number(top.player_id) +
                                         "\nScore: " + QString::number(top.score) +
                                         "\nFinish Time: " + QString::number(top.finish_time));
    }

    void onQueryById() {
        int start = startIdInput->text().toInt();
        int end = endIdInput->text().toInt();

        player_data top = tree.query_the_tree_by_id(start, end);
        if (top.score == -1)
            QMessageBox::information(this, "Result", "No player found in this ID range.");
        else
            QMessageBox::information(this, "Result",
                                     "Top Player:\nID: " + QString::number(top.player_id) +
                                         "\nScore: " + QString::number(top.score) +
                                         "\nFinish Time: " + QString::number(top.finish_time));
    }

    void onUpdatePlayer() {
        int id = updateIdInput->text().toInt();
        int newScore = updateScoreInput->text().toInt();
        int newFinish = updateFinishInput->text().toInt();

        bool ok = tree.update_player_data(player_data(newScore, id, newFinish));
        if (ok) {
            refreshTable();
            QMessageBox::information(this, "Update Success", "Player updated successfully!");
        }
        else
            QMessageBox::warning(this, "Update Failed", "Invalid player ID or update failed.");
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LeaderboardGUI window;
    window.show();

    return app.exec();
}

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QTreeView>
#include <QListView>
#include <QStringList>
#include <QPushButton>
#include <QFileDialog>
#include <QEvent>
#include <QObject>
#include <QMessageBox>

// SelectFileDirDialog
class FileDialog : public QFileDialog{
    Q_OBJECT
private:
    QListView *m_listView;
    QTreeView *m_treeView;
    QPushButton *m_btnOpen;
    QStringList m_selectedFiles;

public slots:
        void chooseClicked();
public:
    FileDialog();
    QStringList selectedFiles();
    bool eventFilter(QObject* watched, QEvent* event);
};
#endif // FILEDIALOG_H

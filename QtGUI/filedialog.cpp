#include "filedialog.h"

FileDialog::FileDialog() : QFileDialog()
{
    m_btnOpen = NULL;
    m_listView = NULL;
    m_treeView = NULL;
    m_selectedFiles.clear();

    this->setOption(QFileDialog::DontUseNativeDialog, true);
    this->setFileMode(QFileDialog::Directory);
    QList<QPushButton*> btns = this->findChildren<QPushButton*>();
    for (int i = 0; i < btns.size(); ++i) {
        QString text = btns[i]->text();
        if (text.toLower().contains("open") || text.toLower().contains("choose"))
        {
            m_btnOpen = btns[i];
            break;
        }
    }

    if (!m_btnOpen) return;

    m_btnOpen->installEventFilter(this);

    m_btnOpen->disconnect(SIGNAL(clicked()));
    connect(m_btnOpen, SIGNAL(clicked()), this, SLOT(chooseClicked()));


    m_listView = findChild<QListView*>("listView");
    if (m_listView) {
        m_listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    m_treeView = findChild<QTreeView*>();
    if (m_treeView) {
        m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

}

bool FileDialog::eventFilter( QObject* watched, QEvent* event )
{
    QPushButton *btn = qobject_cast<QPushButton*>(watched);
    if (btn)
    {
        if(event->type()==QEvent::EnabledChange) {
            if (!btn->isEnabled()) {
                btn->setEnabled(true);
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}


void FileDialog::chooseClicked()
{
    int i =10;
    QModelIndexList indexList = m_listView->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, indexList)
    {
        if (index.column()== 0)
        {
            if(i!=-1)
            {
                if(QFileInfo(this->directory().absolutePath()).fileName()!=index.data().toString())
                {
                    if(this->directory().absolutePath()=="C:/")
                    {
                        if(index.data().toString()!="Windows (C:)")
                        {
                            m_selectedFiles.append(this->directory().absolutePath() + index.data().toString());
                            i--;
                        }
                    }
                    else
                    {
                        m_selectedFiles.append(this->directory().absolutePath() + "/" + index.data().toString());
                        i--;
                    }
                }
            }
            else
            {
                QMessageBox::warning(this, tr("EncryptApp"), tr("Maximum is 10!"));
                return;
            }
        }
    }

    QDialog::accept();
}

QStringList FileDialog::selectedFiles()
{
    return m_selectedFiles;
}

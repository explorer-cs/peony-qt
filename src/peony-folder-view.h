#ifndef PEONYFOLDERVIEW_H
#define PEONYFOLDERVIEW_H

#include <libfm-qt/folderview.h>
#include <libfm-qt/foldermodel.h>

#include <libfm-qt/proxyfoldermodel.h>

class PeonyFolderView : public Fm::FolderView
{
public:
    explicit PeonyFolderView(QWidget *parent = nullptr);

public Q_SLOTS:
    void onFileClicked(int type, const std::shared_ptr<const Fm::FileInfo>& fileInfo) override;

private:
    Fm::FolderModel *m_model = nullptr;
    Fm::ProxyFolderModel *m_proxy_model = nullptr;
};

#endif // PEONYFOLDERVIEW_H

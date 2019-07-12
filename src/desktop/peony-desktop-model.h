#ifndef PEONYDESKTOPMODEL_H
#define PEONYDESKTOPMODEL_H

#include <libfm-qt/foldermodel.h>

class PeonyDesktopModel : public Fm::FolderModel
{
public:
    explicit PeonyDesktopModel();
    //virtual ~PeonyDesktopModel();

public Q_SLOTS:
    void insertComputerHomeTrash();

private:
    bool is_computer_home_trash_added = false;
};

#endif // PEONYDESKTOPMODEL_H

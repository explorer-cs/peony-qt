#include "peony-desktop-model.h"

#include <libfm-qt/core/fileinfojob.h>

PeonyDesktopModel::PeonyDesktopModel() : Fm::FolderModel()
{
    qDebug()<<"init desktop model";
}

void PeonyDesktopModel::insertComputerHomeTrash()
{
    //i have successed adding custom items to model,
    //but there are many potential bugs i have to fix...
    if (is_computer_home_trash_added)
        return;
    Fm::FilePathList paths;
    paths.push_back(Fm::FilePath::fromUri("computer:///"));
    paths.push_back(Fm::FilePath::homeDir());
    paths.push_back(Fm::FilePath::fromUri("trash:///"));

    Fm::FileInfoJob *job = new Fm::FileInfoJob(paths);
    job->runAsync();
    connect(job, &Fm::Job::finished, [=](){
        Fm::FolderModel::insertFiles(0, job->files());
    });

    is_computer_home_trash_added = true;
}

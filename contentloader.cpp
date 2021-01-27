#include "contentloader.h"

ContentLoader::ContentLoader()
{
    QStringList lines = patch_content.split('\n');

    bool started_patch = false;
    bool started_exetype = false;

    QString current_content_name;
    Patch current_patch;
    int current_exetype;
    QVector<Update> current_updates;

    for (int i = 0; i < lines.size(); i++) {
        QString line = lines[i].trimmed();
        if (line.startsWith('>')) {
            if (started_exetype)
                current_patch.content.insert(current_exetype, current_updates);
            if (started_patch)
                this->patches.insert(current_content_name, current_patch);

            started_patch = true;
            current_content_name = line.remove(0, 1);

            started_exetype = false;

        } else if (line.startsWith('|')) {
            if (started_exetype)
                current_patch.content.insert(current_exetype, current_updates);

            started_exetype = true;

            line.remove(0, 1);
            if (line == "simp105")
                current_exetype = ExeType::simp105;
            else if (line == "steam_simp105")
                current_exetype = ExeType::steam_simp105;
            else if (line == "steam_trad105")
                current_exetype = ExeType::steam_trad105;
            current_updates.clear();

        } else {
            bool ok;
            if (line.startsWith("cheat")) {
                QString latter = line.split(":")[1].trimmed();
                QStringList targetstr_split = latter.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                if (targetstr_split.size() != 16)
                    printf("!!FATAL!! bytes not fit for cheat");

                QVector<unsigned char> target;
                for (int i = 0; i < targetstr_split.size(); i++) {
                    target.append(targetstr_split[i].toInt(&ok, 16));
                }
                current_updates.push_back(Update(target));

            } else if (line.contains(',')) {
                int address = line.split(",")[0].toInt(&ok, 16);
                int length = line.split(",")[1].toInt();
                current_updates.push_back(Update(address, length));

            } else if (line.contains(':')) {
                int address = line.split(":")[0].toInt(&ok, 16);

                QString latter = line.split(":")[1].trimmed();
                QStringList lattersplit = latter.split("->");
                QString originstr = lattersplit[0].trimmed();
                QString targetstr = lattersplit[1].trimmed();
                QStringList originstr_split = originstr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                QStringList targetstr_split = targetstr.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                if (originstr_split.size() != targetstr_split.size()) {
                    printf("!!FATAL!! bytes not equal at %x\n", address);
                }
                QVector<unsigned char> origins, target;
                for (int i = 0; i < originstr_split.size(); i++) {
                    origins.append(originstr_split[i].toInt(&ok, 16));
                    target.append(targetstr_split[i].toInt(&ok, 16));
                }
                current_updates.push_back(Update(address, origins, target));

            }
        }
    }
    if (started_exetype)
        current_patch.content.insert(current_exetype, current_updates);
    this->patches.insert(current_content_name, current_patch);
}

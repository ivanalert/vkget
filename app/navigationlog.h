#ifndef NAVIGATIONLOG_H
#define NAVIGATIONLOG_H

#include <QVector>
#include <QObject>

class NavigationLog : public QObject
{
    Q_OBJECT
    //For qml binding.
    Q_PROPERTY(bool hasPrevious READ hasPrevious NOTIFY previousChanged)
    Q_PROPERTY(bool hasNext READ hasNext NOTIFY nextChanged)

public:
    NavigationLog(QObject *parent = nullptr)
        : QObject(parent)
    {

    }

    ~NavigationLog() override = default;

    void record(const int value)
    {
        if (m_log.empty() || m_log.last() != value)
        {
            ++m_pos;
            m_log.remove(m_pos, m_log.size() - m_pos);
            m_log.append(value);
            emit previousChanged();
            emit nextChanged();
        }
    }

    int at(int i)
    {
        return m_log.at(i);
    }

    bool hasPrevious() const
    {
        return m_pos > 0;
    }

    bool hasNext() const
    {
        return m_pos < m_log.size() - 1;
    }

    int previous()
    {
        --m_pos;
        emit previousChanged();
        emit nextChanged();
        return m_log.at(m_pos);
    }

    int next()
    {
        ++m_pos;
        emit nextChanged();
        emit previousChanged();
        return m_log.at(m_pos);
    }

    int home()
    {
        m_pos = 0;
        emit previousChanged();
        emit nextChanged();
        return m_log.at(m_pos);
    }

signals:
    void previousChanged();
    void nextChanged();

private:
    QVector<int> m_log;
    int m_pos = -1;
};

#endif // NAVIGATIONLOG_H

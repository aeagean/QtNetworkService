#include "JsonHttpMethod.h"

JsonHttpMethod::JsonHttpMethod()
{

}

JsonHttpMethod &JsonHttpMethod::header(const QString &key, const QString &value)
{
    m_networkRequest.setRawHeader(QByteArray(key.toStdString().data()), QByteArray(value.toStdString().data()));
    return *this;
}

JsonHttpMethod &JsonHttpMethod::query(const QString &key, const QString &value)
{
    QString urlParam = QString("%1=%2").arg(key),arg(value);
    QString urlString = m_networkRequest.url().toString();
    if (urlString.indexOf("?") == -1)
        m_networkRequest.setUrl(urlString + "?" + urlParam);
    else
        m_networkRequest.setUrl(urlString + "" + urlParam);

    return *this;
}

JsonHttpMethod &JsonHttpMethod::body(const QJsonObject &body)
{
    m_body = body;
    return *this;
}

JsonHttpMethod &JsonHttpMethod::sendRequest(const QObject *respReceiver, const char *slot)
{

}

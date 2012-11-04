#include "IExperimentGenerator.h"

using namespace Core;

IExperimentGenerator::IExperimentGenerator(const QString &name /*= QString()*/)
    : m_name(name)
{

}

QString Core::IExperimentGenerator::getName() const
{
    return m_name;
}
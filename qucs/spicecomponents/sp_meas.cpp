/***************************************************************************
                               sp_meas.cpp
                              -------------
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "sp_meas.h"
#include "extsimkernels/spicecompat.h"

SpiceMeas::SpiceMeas()
{
  isSimulation = true;
  Description = QObject::tr("Measurements");
  Simulator = spicecompat::simNgspice;
  initSymbol(Description);
  Model = ".MEAS";
  Name  = "MEAS";
  SpiceModel = ".MEAS";

  // The description of the first property must stay "Simulation name":
  // the equation editor of the component dialog relies on it.
  Props.append(new Property("Simulation", "TR1", true, "Simulation name"));
  // Measurements: an empty description lets the number of these properties
  // vary (saved as "name=value", like equations)
  Props.append(new Property("vavg", "AVG v(out)", true));
}

Component* SpiceMeas::newOne()
{
  return new SpiceMeas();
}

Element* SpiceMeas::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("Measurements");
  BitmapFile = (char *) "sp_meas";

  if(getNewOne)  return new SpiceMeas();
  return 0;
}

QString SpiceMeas::analysisType(const QString& simModel)
{
  if (simModel == ".TR") return "tran";
  if (simModel == ".AC") return "ac";
  if (simModel == ".SP") return "sp";
  return QString();
}

QString SpiceMeas::getMeasurements(const QString& sim, const QString& analysis,
                                   QStringList& names) const
{
  if (isActive != COMP_IS_ACTIVE) return QString();
  if (!spicecompat::simulationMatches(Props.at(0)->Value, sim)) return QString();

  if (analysis.isEmpty()) {
    // Only complain about an explicit reference; generic ones ("ALL", "TR")
    // simply don't apply to unsupported simulations.
    if (Props.at(0)->Value.toLower() != sim) return QString();
    return QStringLiteral("echo \"Warning: %1: measurements are not supported "
                          "for simulation %2\"\n").arg(Name, sim);
  }

  QString s;
  for (auto pp = Props.begin() + 1; pp != Props.end(); ++pp) {
    QString name = (*pp)->Name.trimmed();
    QString args = (*pp)->Value.trimmed();
    if (name.isEmpty() || args.isEmpty()) continue;
    s += QStringLiteral("meas %1 %2 %3\n").arg(analysis, name, args);
    names.append(name);
  }
  return s;
}

/***************************************************************************
                                sp_meas.h
                               -----------
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SP_MEAS_H
#define SP_MEAS_H

#include "components/component.h"

/*!
 * \brief Ngspice measurements (the `meas` control command) on the results
 *        of a simulation.
 *
 * The first property selects the simulation, every further property is one
 * measurement: the property name is the result name, the value is the rest of
 * the `meas` command after the result name, e.g. `vavg = AVG v(out) from=0`.
 * Results are stored in the dataset as scalars, or as vectors over the swept
 * parameter if the simulation is inside a parameter sweep.
 */
class SpiceMeas : public Component {
public:
  SpiceMeas();
  ~SpiceMeas() override = default;
  Component* newOne() override;
  static Element* info(QString&, char* &, bool getNewOne=false);

  /// @brief Ngspice control script for the measurements on a simulation
  /// @param sim lowercase name of the simulation being netlisted
  /// @param analysis `meas` analysis keyword (tran, ac, sp) for the type of
  /// that simulation, or empty if measurements are not supported for it
  /// @param names receives the names of the measurement results
  /// @return script lines, empty if this component does not apply to \p sim
  QString getMeasurements(const QString& sim, const QString& analysis,
                          QStringList& names) const;

  /// @brief `meas` analysis keyword for a simulation component model,
  /// empty if measurements are not supported for it
  static QString analysisType(const QString& simModel);

protected:
  QString vhdlCode(int) override { return QString(); }
  QString verilogCode(int) override { return QString(); }
  QString netlist() override { return QString(); }
};

#endif

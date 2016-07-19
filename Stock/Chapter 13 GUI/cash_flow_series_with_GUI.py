import numpy as np
import traits.api as trapi
import traitsui.api as trui

class cash_flow_series(trapi.HasTraits):
    name = trapi.Str
    short_rate = trapi.Range(0.0, 0.5, 0.05)
    time_list = trapi.Array(dtype = np.float, shape = (1, 6))
    disc_value = trapi.Array(dtype = np.float, shape = (1, 6))
    cash_flows = trapi.Array(dtype = np.float, shape = (1, 6))
    present_values = trapi.Array(dtype = np.float, shape = (1, 6))
    net_present_value = trapi.Float
    update = trapi.Button
    def _update_fired(self):
        self.disc_value = np.exp(-self.short_rate * self.time_list)
        self.present_values = self.disc_value * self.cash_flows
        self.net_present_value = np.sum(self.present_values)
    v = trui.View(trui.Group(trui.Item(name = 'name'),
                             trui.Item(name = 'short_rate'),
                             trui.Item(name = 'time_list', label = 'Time List'),
                             trui.Item(name = 'cash_flows', label = 'CashFlows'),
                             trui.Item('update', show_label = False),
                             trui.Item(name = 'disc_value', label = 'Discount Values'),
                             trui.Item(name = 'present_values', label = 'Present Values'),
                             trui.Item(name = 'net_present_value', label = 'Net Present Value'),
                             show_border = True, label = 'Calculate Present Value'),
                  buttons = [trui.OKButton, trui.CancelButton],
                  resizable = True)

if __name__ == '__main__':
    cfs = cash_flow_series()
    cfs.configure_traits()

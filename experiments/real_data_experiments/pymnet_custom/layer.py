from matplotlib.patches import Rectangle
from pymnet.visuals.drawbackends.mpl import LayerMPL, fix_attr
from mpl_toolkits.mplot3d import art3d


class LayerMplCustom(LayerMPL):
    def draw(self):
        assert self.z != None
        self.layer = Rectangle((0, 0), 1, 1, alpha=self.alpha, color=self.color)
        if self.label is not None:
            txt = self.labelObject = self.net.ax.text(
                self.labelloc[0],
                self.labelloc[1],
                self.z,
                fr'$\mathrm{{{self.label}}}$',
                **self.labelArgs,
            )
            art3d.text_2d_to_3d(txt, z=0, zdir='x')
        self.net.ax.add_patch(self.layer)
        art3d.pathpatch_2d_to_3d(self.layer, z=self.z, zdir='z')
        fix_attr(self.layer, 'zorder', self.z)


env.DrawPoint = function(x, y, sz, style, len) {
    ctx.fillStyle = IO.decode(style, len);
    ctx.fillRect(x,y,sz,sz);
}
env.Clear = function() {
    ctx.fillStyle = "#FFF";
    ctx.fillRect(0, 0, 1280, 720);
}
env.Init = function() {
    const canvas = document.querySelector("canvas");
    window.ctx = canvas.getContext("2d", {alpha: false});
    env.Clear();
}
window.setMessageCallback('getFps', function(name, args) {
	$("#fps").text( "FPS: " + args[0] );
});
window.setMessageCallback('getX', function(name, args) {
	$("#posX").text( "(" + Number((args[0]).toFixed(1)) + ", " );
});
window.setMessageCallback('getY', function(name, args) {
	$("#posY").text( Number((args[0]).toFixed(1)) + ", " );
});
window.setMessageCallback('getZ', function(name, args) {
	$("#posZ").text( Number((args[0]).toFixed(1)) + ")" );
});

/*
window.setMessageCallback('getThreadPoolQueueSize', function(name, args) {
	$("#threadPoolQueueSize").text( Number((args[0]).toFixed(1)) );
});
*/

window.setMessageCallback('getThreadPoolWorkerCount', function(name, args) {
	$("#threadPoolWorkerCount").text( Number((args[0]).toFixed(1)) );
});
window.setMessageCallback('getThreadPoolActiveWorkerCount', function(name, args) {
	$("#threadPoolActiveWorkerCount").text( Number((args[0]).toFixed(1)) );
});
window.setMessageCallback('getThreadPoolInactiveWorkerCount', function(name, args) {
	$("#threadPoolInactiveWorkerCount").text( Number((args[0]).toFixed(1)) );
});

/*
window.setMessageCallback('getOpenGlThreadPoolQueueSize', function(name, args) {
	$("#openGlQueueCount").text( Number((args[0]).toFixed(1)) );
});
*/

window.setMessageCallback('getTimeForPhysics', function(name, args) {
	$("#timeForPhysics").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForTick', function(name, args) {
	$("#timeForTick").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForRender', function(name, args) {
	$("#timeForRender").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForMisc', function(name, args) {
	$("#timeForMisc").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForInput', function(name, args) {
	$("#timeForInput").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForGuiUpdate', function(name, args) {
	$("#timeForGuiUpdate").text( Number((args[0]).toFixed(1)) + "ms" );
});
window.setMessageCallback('getTimeForAngelScript', function(name, args) {
	$("#timeForAngelScript").text( Number((args[0]).toFixed(1)) + "ms" );
});

class Dashing.State extends Dashing.Widget
	onData: (data) ->		
		$(@get('node')).removeClass('state-0')
					   .removeClass('state-1')
					   .addClass("state-#{data.value}")